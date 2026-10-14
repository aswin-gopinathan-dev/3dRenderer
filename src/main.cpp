#include <iostream>
#include <SDL2/SDL.h>
#include "Display.h"
#include "Vector.h"
#include <vector>

bool is_running = true;
Display g_display;
std::vector<Vec3D> g_cube_points;
std::vector<Vec2D> g_projected_points;
Vec3D camera_pos {0, 0, -5};
Vec3D cube_rotation {};
float fov_factor = 640;


void setup()
{
    g_cube_points.resize(9*9*9);
    g_projected_points.resize(9*9*9);

    for(float x=-1; x<=1; x += 0.25)
    {
        for(float y=-1; y<=1; y += 0.25)
        {
            for(float z=-1; z<=1; z += 0.25)
            {
                Vec3D pt {x, y, z};
                g_cube_points.push_back(pt);
            }
        }        
    }
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
    g_projected_points.clear();
    for (auto v : g_cube_points)
    {
        v.Rotate(Axis::X_AXIS, cube_rotation.x);
        v.Rotate(Axis::Y_AXIS, cube_rotation.y);
        v.Rotate(Axis::Z_AXIS, cube_rotation.z);
        
        // Move the point away from camera
        v.z -= camera_pos.z;
        Vec2D projected_point = project(v);
        g_projected_points.push_back(projected_point);
    }
}


void render(void) 
{
    g_display.ClearColorBuffer(0xFF000000);    
    g_display.DrawGrid();

    for (auto v : g_projected_points)
    {
        Vec2D point = v;
        g_display.DrawRect(v.x + g_display.GetWindowWidth()/2, 
                           v.y + g_display.GetWindowHeight()/2, 
                           4, 4, 0xFF0000FF);
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
