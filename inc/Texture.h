#pragma once
#include <cstdint>
#include "upng.h"


class Texture
{
public:
    Texture();
    Texture(float uu, float vv);
    static int width;
    static int height;

    float u;
    float v;
};

class TextureData
{
public:
    void LoadTextureData(const char* file);

    upng_t* png_texture;
    uint32_t* mesh_texture;
};

extern const uint8_t REDBRICK_TEXTURE[];
extern uint32_t* mesh_texture;