#include <iostream>
#include <SDL2/SDL.h>
#include "Display.h"

bool is_running = true;
Display g_display;

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

void update(void) 
{
    
}

void render(void) 
{
    g_display.ClearColorBuffer(0xFF000000);    
    g_display.DrawGrid();
    g_display.DrawRect(300, 200, 300, 150, 0xFFFF00FF);

    g_display.RenderColorBuffer();
}

int main()
{
    while (is_running) 
    {
        process_input();
        update();
        render();
    }
	return 0;
}
