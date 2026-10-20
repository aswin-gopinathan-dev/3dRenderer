#include <iostream>
#include <SDL2/SDL.h>
#include "Display.h"
#include "Vector.h"
#include "Triangle.h"
#include "Mesh.h"
#include <vector>
#include <algorithm>

bool is_running = true;
Display g_display;
Mesh g_mesh;
Vec3D camera_pos {0, 0, 0};
Vec3D cube_rotation {};
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
    cube_rotation.x += 0.002;
    cube_rotation.y += 0.002;
    cube_rotation.z += 0.002;

    triangles_to_render.clear();
    std::vector<Vec3D> vertices = g_mesh.GetMeshVertices();
    std::vector<Face> faces = g_mesh.GetMeshFaces();
    for(auto face : faces)
    {
        Vec3D face_vertices[3];
        face_vertices[0] = vertices[face.a-1];
        face_vertices[1] = vertices[face.b-1];
        face_vertices[2] = vertices[face.c-1];
        
        std::vector<Vec3D> transformed_vertices;
        transformed_vertices.reserve(3);
        for (auto vertex : face_vertices)
        {
            vertex.Rotate(Axis::X_AXIS, cube_rotation.x);
            vertex.Rotate(Axis::Y_AXIS, cube_rotation.y);
            vertex.Rotate(Axis::Z_AXIS, cube_rotation.z);

            // Move the point away from camera
            vertex.z += 5;
            transformed_vertices.push_back(vertex);
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
            Vec2D projected_point = project(vertex);
            projected_point.x += g_display.GetWindowWidth() / 2;
            projected_point.y += g_display.GetWindowHeight() / 2;
            projected_points.push_back(projected_point);
        }

        Triangle triangle;
        std::copy(projected_points.begin(), projected_points.end(), triangle.points);
        triangle.color = face.color;
        triangles_to_render.push_back(triangle);
    }
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
