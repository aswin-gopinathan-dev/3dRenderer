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

float Vec2D::Magnitude()
{
    return sqrt(x*x + y*y);
}

Vec2D Vec2D::Add(Vec2D& v)
{
    Vec2D result;
    result.x = x + v.x;
    result.y = y + v.y;
    return result;
}

Vec2D Vec2D::Sub(Vec2D& v)
{
    Vec2D result;
    result.x = x - v.x;
    result.y = y - v.y;
    return result;
}

Vec2D Vec2D::Multiply(float s)
{
    Vec2D result;
    result.x = x * s;
    result.y = y * s;
    return result;
}


Vec2D Vec2D::Div(float s)
{
    Vec2D result;
    result.x = x / s;
    result.y = y / s;
    return result;
}


float Vec2D::Dot(Vec2D& v)
{
    float result = x * v.x + y * v.y;
    return result;
}

void Vec2D::Normalize()
{
    float mag = Magnitude();
    x = x / mag;
    y = y / mag;
}

///////////////////////////////////////////////////////////////////////

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

float Vec3D::Magnitude()
{
    return sqrt(x*x + y*y + z*z);
}

Vec3D Vec3D::Add(Vec3D& v)
{
    Vec3D result;
    result.x = x + v.x;
    result.y = y + v.y;
    result.z = z + v.z;
    return result;
}

Vec3D Vec3D::Sub(Vec3D& v)
{
    Vec3D result;
    result.x = x - v.x;
    result.y = y - v.y;
    result.z = z - v.z;
    return result;
}

Vec3D Vec3D::Multiply(float s)
{
    Vec3D result;
    result.x = x * s;
    result.y = y * s;
    result.z = z * z;
    return result;
}

Vec3D Vec3D::Div(float s)
{
    Vec3D result;
    result.x = x / s;
    result.y = y / s;
    result.z = z / z;
    return result;
}

Vec3D Vec3D::Cross(Vec3D& v)
{
    Vec3D result;
    result.x = y * v.z - z * v.y;
    result.y = z * v.x - x * v.z;
    result.z = x * v.y - y * v.x;
    return result;
}

float Vec3D::Dot(Vec3D& v)
{
    float result = x * v.x + y * v.y + z * v.z;
    return result;
}

void Vec3D::Normalize()
{
    float mag = Magnitude();
    x = x / mag;
    y = y / mag;
    z = z / mag;
}
