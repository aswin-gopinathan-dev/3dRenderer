#include "Display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

Display::Display()
{
    // Initlaize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        throw std::runtime_error("SDL Initialization failed");


    // Get display info
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    m_windowWidth = displayMode.w;
    m_windowHeight = displayMode.h;

    // Create SDL Window
    m_window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                m_windowWidth, m_windowHeight, SDL_WINDOW_BORDERLESS);
    if(!m_window)
        throw std::runtime_error("SDL Window creation failed");

    // Create SDL Renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if(!m_renderer)
        throw std::runtime_error("SDL Renderer creation failed");

    // Create color buffer and SDL Texture
    m_colorBuffer = new uint32_t[m_windowWidth * m_windowHeight];
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                             m_windowWidth, m_windowHeight);
}


Display::~Display()
{
    delete[] m_colorBuffer;
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    //SDL_DestroyWindow(m_window);
    SDL_Quit();
}


void Display::ClearColorBuffer(uint32_t color)
{
    for (int y = 0; y < m_windowHeight; y++) 
    {
        for (int x = 0; x < m_windowWidth; x++) 
        {
            m_colorBuffer[(m_windowWidth * y) + x] = color;
        }
    }
}


void Display::RenderColorBuffer()
{
    SDL_UpdateTexture(m_texture, NULL, m_colorBuffer, int(m_windowWidth * sizeof(uint32_t)));
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);    
}


void Display::DrawGrid()
{
    for (int y = 0; y < m_windowHeight; y += 10) 
    {
        for (int x = 0; x < m_windowWidth; x += 10) 
        {
            m_colorBuffer[(m_windowWidth * y) + x] = 0xFF444444;
        }
    }
}


void Display::DrawRect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < width; i++) 
    {
        for (int j = 0; j < height; j++) 
        {
            int current_x = x + i;
            int current_y = y + j;
            DrawPixel(current_x, current_y, color);
        }
    }
}


void Display::DrawPixel(int x, int y, uint32_t color)
{
    if( x >=0 && x < m_windowWidth && y >= 0 && y < m_windowHeight )
        m_colorBuffer[(m_windowWidth * y) + x] = color;
}


void Display::DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int longest_side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);
    float x_inc = delta_x / (float)longest_side_length;
    float y_inc = delta_y / (float)longest_side_length;

    float current_x = x0;
    float current_y = y0;

    for(int i = 0; i <= longest_side_length; i++)
    {
        DrawPixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}


void Display::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    DrawLine(x0, y0, x1, y1, color);
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x0, y0, color);
}
