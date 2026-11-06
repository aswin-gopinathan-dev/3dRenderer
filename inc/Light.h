#pragma once
#include "Vector.h"
#include <cstdint>

class Light
{
public:
    Light();
    Light(Vec3D dir);
    ~Light();

    uint32_t GetLightIntensity(uint32_t color, float factor);

    Vec3D direction;
};