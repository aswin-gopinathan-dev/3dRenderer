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
Vec3D camera_pos {0, 0, -5};
Vec3D cube_rotation {};
float fov_factor = 640;
std::vector<Triangle> triangles_to_render;


void setup()
{
    //g_mesh.CreateCubeMesh();
    g_mesh.CreateMeshFromObjFile("./assets/f22.obj");
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
    cube_rotation.x += 0.005;
    cube_rotation.y += 0.005;
    cube_rotation.z += 0.005;

    triangles_to_render.clear();
    std::vector<Vec3D> vertices = g_mesh.GetMeshVertices();
    std::vector<Face> faces = g_mesh.GetMeshFaces();
    for(auto face : faces)
    {
        Vec3D face_vertices[3];
        face_vertices[0] = vertices[face.a-1];
        face_vertices[1] = vertices[face.b-1];
        face_vertices[2] = vertices[face.c-1];
        
        std::vector<Vec2D> projected_points;
        projected_points.reserve(3);
        for (auto vertex : face_vertices)
        {
            vertex.Rotate(Axis::X_AXIS, cube_rotation.x);
            vertex.Rotate(Axis::Y_AXIS, cube_rotation.y);
            vertex.Rotate(Axis::Z_AXIS, cube_rotation.z);

            // Move the point away from camera
            vertex.z -= camera_pos.z;
            Vec2D projected_point = project(vertex);
            projected_point.x += g_display.GetWindowWidth() / 2;
            projected_point.y += g_display.GetWindowHeight() / 2;
            projected_points.push_back(projected_point);
        }

        Triangle triangle;
        std::copy(projected_points.begin(), projected_points.end(), triangle.points);
        triangles_to_render.push_back(triangle);
    }
}


void render(void) 
{
    g_display.ClearColorBuffer(0xFF000000);    
    g_display.DrawGrid();

    for (auto triangle : triangles_to_render)
    {
        g_display.DrawRect(triangle.points[0].x, triangle.points[0].y , 4, 4, 0xFF0000FF); 
        g_display.DrawRect(triangle.points[1].x, triangle.points[1].y , 4, 4, 0xFF0000FF); 
        g_display.DrawRect(triangle.points[2].x, triangle.points[2].y , 4, 4, 0xFF0000FF); 
        g_display.DrawTriangle(triangle.points[0].x, triangle.points[0].y, 
                               triangle.points[1].x, triangle.points[1].y, 
                               triangle.points[2].x, triangle.points[2].y, 0xFF0000FF);
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
