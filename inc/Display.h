#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <SDL2/SDL.h>
#include "Texture.h"
#include "Vector.h"


class Display
{
public:
    Display();
	~Display();

    void RenderColorBuffer();
    void ClearColorBuffer(uint32_t color);

    void DrawGrid();
    void DrawRect(int x, int y, int width, int height, uint32_t color);
    void DrawPixel(int x, int y, uint32_t color);
    void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);
    void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    void DrawTrianglePixel(int x, int y, uint32_t color, Vec4D point_a, Vec4D point_b, Vec4D point_c);
    void DrawFilledTriangle(Vec4D v0, Vec4D v1, Vec4D v2, uint32_t color);
    void DrawTexturedTriangle(int x0, int y0, float z0, float w0, Texture uv0, int x1, int y1, float z1, float w1, Texture uv1, int x2, int y2, float z2, float w2, Texture uv2, uint32_t* texture);
    void FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    void FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    Vec3D BarycentricWeights(Vec2D a, Vec2D b, Vec2D c, Vec2D p);
    void DrawTexel(int x, int y, uint32_t* texture, Vec4D point_a, Vec4D point_b, Vec4D point_c,
                   Texture uv0, Texture uv1, Texture uv2);
    

    SDL_Renderer* GetRenderer() { return m_renderer; }
    SDL_Texture* GetTexture() { return m_texture; }
    SDL_Window* GetWindow() { return m_window; }
    int GetWindowWidth() { return m_windowWidth; }
    int GetWindowHeight() { return m_windowHeight; }
    float* GetZBuffer() { return m_zBuffer; }

    void ClearZBuffer();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    uint32_t* m_colorBuffer;
    float* m_zBuffer;
    SDL_Texture* m_texture;
    int m_windowWidth;
    int m_windowHeight;
};
	
