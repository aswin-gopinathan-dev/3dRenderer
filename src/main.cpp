#include <iostream>
#include <SDL2/SDL.h>
#include "Display.h"
#include "Vector.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Matrix.h"
#include <vector>
#include <algorithm>

bool is_running = true;
Display g_display;
Mesh g_mesh;
Vec3D camera_pos {0, 0, 0};
float fov_factor = 640;
std::vector<Triangle> triangles_to_render;

enum class CullMethod
{
    CULL_NONE,
    CULL_BACKFACE
};

enum class RenderMethod
{
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE
};

CullMethod g_cullMethod;
RenderMethod g_renderMethod;

void setup()
{
    g_renderMethod = RenderMethod::RENDER_WIRE;
    g_cullMethod = CullMethod::CULL_BACKFACE;
    g_mesh.CreateCubeMesh();
    //g_mesh.CreateMeshFromObjFile("./assets/cube.obj");
}

void process_input(void) 
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) 
    {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            if (event.key.keysym.sym == SDLK_1)
                g_renderMethod = RenderMethod::RENDER_WIRE_VERTEX;
            if (event.key.keysym.sym == SDLK_2)
                g_renderMethod = RenderMethod::RENDER_WIRE;
            if (event.key.keysym.sym == SDLK_3)
                g_renderMethod = RenderMethod::RENDER_FILL_TRIANGLE;
            if (event.key.keysym.sym == SDLK_4)
                g_renderMethod = RenderMethod::RENDER_FILL_TRIANGLE_WIRE;
            if (event.key.keysym.sym == SDLK_c)
                g_cullMethod = CullMethod::CULL_BACKFACE;
            if (event.key.keysym.sym == SDLK_d)
                g_cullMethod = CullMethod::CULL_NONE;
            break;
    }
}


Vec2D project(Vec3D& pt)
{
    Vec2D v {fov_factor * pt.x / pt.z, fov_factor * pt.y / pt.z};
    return v;
}


void update(void) 
{
    g_mesh.rotation.x += 0.002;
    g_mesh.rotation.y += 0.002;
    g_mesh.rotation.z += 0.002;
    //g_mesh.scale.x += 0.002;
    //g_mesh.translation.x += 0.01;
    g_mesh.translation.z = 5.0;

    Matrix scale_matrix;
    scale_matrix.Scale(g_mesh.scale.x, g_mesh.scale.y, g_mesh.scale.z);
    Matrix translation_matrix;
    translation_matrix.Translate(g_mesh.translation.x, g_mesh.translation.y, g_mesh.translation.z);
    Matrix rotation_matrix_x;
    rotation_matrix_x.RotateX(g_mesh.rotation.x);
    Matrix rotation_matrix_y;
    rotation_matrix_y.RotateY(g_mesh.rotation.y);
    Matrix rotation_matrix_z;
    rotation_matrix_z.RotateZ(g_mesh.rotation.z);

    Matrix world_matrix;
    world_matrix = world_matrix.Multiply(translation_matrix);
    world_matrix = world_matrix.Multiply(rotation_matrix_x);
    world_matrix = world_matrix.Multiply(rotation_matrix_y);
    world_matrix = world_matrix.Multiply(rotation_matrix_z);
    world_matrix = world_matrix.Multiply(scale_matrix);

    triangles_to_render.clear();
    std::vector<Vec3D> vertices = g_mesh.GetMeshVertices();
    std::vector<Face> faces = g_mesh.GetMeshFaces();
    for(auto face : faces)
    {
        Vec3D face_vertices[3];
        face_vertices[0] = vertices[face.a-1];
        face_vertices[1] = vertices[face.b-1];
        face_vertices[2] = vertices[face.c-1];
        
        std::vector<Vec4D> transformed_vertices;
        transformed_vertices.reserve(3);
        for (auto vertex : face_vertices)
        {
            Vec4D transformed_vertex = world_matrix.Multiply(vertex);
            transformed_vertices.push_back(transformed_vertex);
        }

        //Backface culling
        if(g_cullMethod == CullMethod::CULL_BACKFACE)
         {
            Vec3D A = transformed_vertices[0];
            Vec3D B = transformed_vertices[1];
            Vec3D C = transformed_vertices[2];
            Vec3D AB = B.Sub(A);
            Vec3D AC = C.Sub(A);

            // Get face normal
            Vec3D normal = AB.Cross(AC);
            normal.Normalize();

            // Find camera ray
            Vec3D camera_ray = camera_pos.Sub(A);

            // Find how aligned is camera ray with face normal
            float dot = camera_ray.Dot(normal);

            if(dot < 0)
                continue;
        }

        // Project vertices
        std::vector<Vec2D> projected_points;
        projected_points.reserve(3);
        for (auto vertex : transformed_vertices)        
        {
            Vec3D v3 = vertex;
            Vec2D projected_point = project(v3);
            projected_point.x += g_display.GetWindowWidth() / 2;
            projected_point.y += g_display.GetWindowHeight() / 2;
            projected_points.push_back(projected_point);
        }

        Triangle triangle;
        std::copy(projected_points.begin(), projected_points.end(), triangle.points);
        triangle.color = face.color;
        triangle.avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;
        triangles_to_render.push_back(triangle);
    }

    std::sort(triangles_to_render.begin(), triangles_to_render.end(), [](Triangle a, Triangle b) { return a.avg_depth > b.avg_depth;});
}


void render(void) 
{
    g_display.ClearColorBuffer(0xFF000000);    
    g_display.DrawGrid();

    for (auto triangle : triangles_to_render)
    {
        if(g_renderMethod == RenderMethod::RENDER_FILL_TRIANGLE || 
           g_renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE)
        {
            g_display.DrawFilledTriangle(triangle.points[0].x, triangle.points[0].y, 
                                         triangle.points[1].x, triangle.points[1].y, 
                                         triangle.points[2].x, triangle.points[2].y, 
                                         triangle.color);
        }

        if(g_renderMethod == RenderMethod::RENDER_WIRE || 
           g_renderMethod == RenderMethod::RENDER_WIRE_VERTEX || 
           g_renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE)
        {
            g_display.DrawTriangle(triangle.points[0].x, triangle.points[0].y, 
                                   triangle.points[1].x, triangle.points[1].y, 
                                   triangle.points[2].x, triangle.points[2].y, 
                                   0xFFFFFFFF);
        }

        if(g_renderMethod == RenderMethod::RENDER_WIRE_VERTEX)
        {
            g_display.DrawRect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFFFFFF); 
            g_display.DrawRect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFFFFFF); 
            g_display.DrawRect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFFFFFF); 
        }
    }

    g_display.RenderColorBuffer();
}

int main()
{
    setup();

    while (is_running) 
    {
        process_input();
        update();
        render();
    }
	return 0;
}
