#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <SDL2/SDL.h>


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
    

    SDL_Renderer* GetRenderer() { return m_renderer; }
    SDL_Texture* GetTexture() { return m_texture; }
    SDL_Window* GetWindow() { return m_window; }
    int GetWindowWidth() { return m_windowWidth; }
    int GetWindowHeight() { return m_windowHeight; }

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    uint32_t* m_colorBuffer;
    SDL_Texture* m_texture;
    int m_windowWidth;
    int m_windowHeight;
};
	
