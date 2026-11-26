#pragma once
#include "Vector.h"

class Camera
{
public:
    Vec3D position;
    Vec3D direction;
    Vec3D forward_velocity;
    float yaw;

    Camera();
    ~Camera();
};