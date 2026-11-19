#pragma once
#include <cstdint>


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

extern const uint8_t REDBRICK_TEXTURE[];
extern uint32_t* mesh_texture;