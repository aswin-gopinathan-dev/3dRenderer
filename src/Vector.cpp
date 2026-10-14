#include "../inc/Vector.h"
#include <math.h>

Vec2D::Vec2D() : x(0.0), y(0.0)
{
}

Vec2D::Vec2D(float a, float b) : x(a), y(b)
{

}

Vec2D::~Vec2D()
{
}


Vec3D::Vec3D() : x(0.0), y(0.0), z(0.0)
{
}

Vec3D::Vec3D(float a, float b, float c) : x(a), y(b), z(c)
{

}

Vec3D::~Vec3D()
{
}

void Vec3D::Rotate(Axis axis, float angle)
{
    Vec3D result;
    switch (axis)
    {
        case Axis::X_AXIS:
            result.x = x;
            result.y = y * cos(angle) - z * sin(angle);
            result.z = y * sin(angle) + z * cos(angle);
            break;
        case Axis::Y_AXIS:
            result.x = x * cos(angle) - z * sin(angle);
            result.y = y;
            result.z = x * sin(angle) + z * cos(angle);
            break;
        case Axis::Z_AXIS:
            result.x = x * cos(angle) - y * sin(angle);
            result.y = x * sin(angle) + y * cos(angle);
            result.z = z;
            break;
    }
    
    *this = result;
}
