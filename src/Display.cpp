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


void Display::FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Slope equation is inverted as we need to find x for each increment of y
    float slope1 = (float)(x1 - x0) / (y1 - y0);
    float slope2 = (float)(x2 - x0) / (y2 - y0);

    float x_start = x0;
    float x_end = x0;
    // Loop scanlines from top to bottom
    for(int y = y0; y <= y1; y++)
    {
        DrawLine(x_start, y, x_end, y, color);
        x_start += slope1;
        x_end += slope2;
    }
}


void Display::FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Slope equation is inverted as we need to find x for each increment of y
    float slope1 = (float)(x2 - x0) / (y2 - y0);
    float slope2 = (float)(x2 - x1) / (y2 - y1);

    float x_start = x2;
    float x_end = x2;
    // Loop scanlines from top to bottom
    for(int y = y2; y >= y0; y--)
    {
        DrawLine(x_start, y, x_end, y, color);
        x_start -= slope1;
        x_end -= slope2;
    }
}


void Display::DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Sort vertices by ascending Y coordinate
    if (y0 > y1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    if (y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    if (y0 > y1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    if(y1 == y2)
    {
        FillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if(y0 == y1)
    {
        FillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        int My = y1;
        int Mx = (x2 - x0) * (y1 - y0) / (float)(y2 - y0) + x0;

        FillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);
        FillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);
    }
}


void Display::DrawTexel(int x, int y, uint32_t* texture, Vec2D point_a, Vec2D point_b, Vec2D point_c,
                        Texture uv0, Texture uv1, Texture uv2)
{
    Vec2D point_p(x, y);
    Vec3D weights = BarycentricWeights(point_a, point_b, point_c, point_p);
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolated_u = uv0.u * alpha + uv1.u * beta + uv2.u * gamma;
    float interpolated_v = uv0.v * alpha + uv1.v * beta + uv2.v * gamma;

    int tex_x = abs((int)(interpolated_u * uv0.width));
    int tex_y = abs((int)(interpolated_v * uv0.width));
    if(tex_x >=0 && tex_y >= 0 && tex_x < uv0.width && tex_y < uv0.height)
        DrawPixel(x, y, texture[tex_x * uv0.width + tex_y]);
}


void Display::DrawTexturedTriangle(int x0, int y0, Texture uv0, int x1, int y1, Texture uv1, int x2, int y2, Texture uv2, uint32_t* texture)
{
    // Sort vertices by ascending Y coordinate
    if (y0 > y1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(uv0, uv1);
    }

    if (y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
        std::swap(uv1, uv2);
    }

    if (y0 > y1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(uv0, uv1);
    }

    Vec2D point_a(x0, y0);
    Vec2D point_b(x1, y1);
    Vec2D point_c(x2, y2);

    float inv_slope1 = 0;
    float inv_slope2 = 0;

    if (y1 - y0 != 0) inv_slope1 = (float)(x1 - x0) / abs(y1 - y0);
    if( y2 - y0 !=0 ) inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);

    if(y1 - y0 != 0)
    {
        for(int y = y0; y <= y1; y++)
        {
            int x_start = x1 + (y - y1) * inv_slope1;
            int x_end = x0 + (y - y0) * inv_slope2;
            if(x_end < x_start)
                std::swap(x_start, x_end);

            for(int x = x_start; x < x_end; x++)
            {
                DrawTexel(x, y, texture, point_a, point_b, point_c, uv0, uv1, uv2);
            }
        }
    }


    inv_slope1 = 0;
    inv_slope2 = 0;

    if (y2 - y1 != 0) inv_slope1 = (float)(x2 - x1) / abs(y2 - y1);
    if( y2 - y0 !=0 ) inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);

    if(y2 - y1 != 0)
    {
        for(int y = y1; y <= y2; y++)
        {
            int x_start = x1 + (y - y1) * inv_slope1;
            int x_end = x0 + (y - y0) * inv_slope2;
            if(x_end < x_start)
                std::swap(x_start, x_end);

            for(int x = x_start; x < x_end; x++)
            {
                DrawTexel(x, y, texture, point_a, point_b, point_c, uv0, uv1, uv2);
            }
        }
    }
}


Vec3D Display::BarycentricWeights(Vec2D a, Vec2D b, Vec2D c, Vec2D p)
{
    Vec2D ac = c.Sub(a);
    Vec2D ab = b.Sub(a);
    Vec2D pc = c.Sub(p);
    Vec2D pb = b.Sub(p);
    Vec2D ap = p.Sub(a);

    float area_parallelogram_abc = (ac.x*ab.y - ac.y*ab.x);
    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;
    float gamma = 1.0 - alpha - beta;

    Vec3D weights(alpha, beta, gamma);
    return weights;
}