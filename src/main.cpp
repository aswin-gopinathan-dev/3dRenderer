#include <iostream>
#include <SDL2/SDL.h>
#include "Display.h"
#include "Vector.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Light.h"
#include "Texture.h"
#include <vector>
#include <algorithm>
#include "upng.h"

bool is_running = true;
Display g_display;
Mesh g_mesh;
Vec3D camera_pos {0, 0, 0};
Matrix projection_matrix;
Light g_light(Vec3D(0, 0, 1));
TextureData g_textureData;

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
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_TEXTURED,
    RENDER_TEXTURED_WIRE
};

CullMethod g_cullMethod;
RenderMethod g_renderMethod;

void setup()
{
    g_renderMethod = RenderMethod::RENDER_WIRE;
    g_cullMethod = CullMethod::CULL_BACKFACE;
    //g_mesh.CreateCubeMesh();
    g_mesh.CreateMeshFromObjFile("./assets/f22.obj");
    const char* asset = "./assets/f22.png";
    g_textureData.LoadTextureData(asset);
    mesh_texture = g_textureData.mesh_texture;
     

    float fov = M_PI / 3.0;
    float aspect = (float)g_display.GetWindowHeight() / g_display.GetWindowWidth();
    float zfar = 100.0;
    float znear = 0.1;
    projection_matrix = projection_matrix.ProjectionMatrix(fov, aspect, znear, zfar);
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
            if (event.key.keysym.sym == SDLK_5)
                g_renderMethod = RenderMethod::RENDER_TEXTURED;
            if (event.key.keysym.sym == SDLK_6)
                g_renderMethod = RenderMethod::RENDER_TEXTURED_WIRE;
            if (event.key.keysym.sym == SDLK_c)
                g_cullMethod = CullMethod::CULL_BACKFACE;
            if (event.key.keysym.sym == SDLK_d)
                g_cullMethod = CullMethod::CULL_NONE;
            break;
    }
}


void update(void) 
{
    g_mesh.rotation.x += 0.005;
    g_mesh.rotation.y += 0.005;
    g_mesh.rotation.z += 0.005;
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

        if(g_cullMethod == CullMethod::CULL_BACKFACE)
        {
            if(dot < 0)
                continue;
        }

        // Project vertices
        std::vector<Vec4D> projected_points;
        projected_points.reserve(3);
        for (auto vertex : transformed_vertices)        
        {
            Vec3D v3 = vertex;
            Vec4D projected_point = projection_matrix.Project(vertex);

            projected_point.x *= (g_display.GetWindowWidth() / 2);
            projected_point.y *= (g_display.GetWindowHeight() / 2);

            // Invert y values to account for flipped screen y coordinate (compared to obj)
            projected_point.y *= -1;

            projected_point.x += g_display.GetWindowWidth() / 2;
            projected_point.y += g_display.GetWindowHeight() / 2;

            Vec4D point(projected_point.x, projected_point.y, projected_point.z, projected_point.w);
            projected_points.push_back(point);
        }

        Triangle triangle;
        std::copy(projected_points.begin(), projected_points.end(), triangle.points);

        float light_intensity_factor = -normal.Dot(g_light.direction);
        uint32_t triangle_color = g_light.GetLightIntensity(face.color, light_intensity_factor);

        triangle.color = triangle_color;
        triangle.tex[0] = face.a_uv;
        triangle.tex[1] = face.b_uv;
        triangle.tex[2] = face.c_uv;
        triangles_to_render.push_back(triangle);
    }
}


void render(void) 
{
    g_display.ClearColorBuffer(0xFF000000);    
    g_display.ClearZBuffer();
    g_display.DrawGrid();

    for (auto triangle : triangles_to_render)
    {
        if(g_renderMethod == RenderMethod::RENDER_FILL_TRIANGLE || 
           g_renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE)
        {
            g_display.DrawFilledTriangle(triangle.points[0], 
                                         triangle.points[1], 
                                         triangle.points[2], 
                                         triangle.color);
        }

        if(g_renderMethod == RenderMethod::RENDER_WIRE || 
           g_renderMethod == RenderMethod::RENDER_WIRE_VERTEX || 
           g_renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE || 
           g_renderMethod == RenderMethod::RENDER_TEXTURED_WIRE)
        {
            g_display.DrawTriangle(triangle.points[0].x, triangle.points[0].y, 
                                   triangle.points[1].x, triangle.points[1].y, 
                                   triangle.points[2].x, triangle.points[2].y, 
                                   0xFFFFFFFF);
        }

        if(g_renderMethod == RenderMethod::RENDER_TEXTURED || 
           g_renderMethod == RenderMethod::RENDER_TEXTURED_WIRE)
        {
            g_display.DrawTexturedTriangle(triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.tex[0], 
                                           triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.tex[1], 
                                           triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.tex[2], 
                                           mesh_texture);
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
