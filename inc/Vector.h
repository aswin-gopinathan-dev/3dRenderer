#pragma once
#include <string>
#include <iostream>

class Vec2D
{
public:
	Vec2D();
    Vec2D(float a, float b);
	~Vec2D();
    void print(std::string msg = "") { std::cout << msg << " " << x << ", " << y << std::endl; }    

    float Magnitude();
    Vec2D Add(Vec2D& v);
    Vec2D Sub(Vec2D& v);
    Vec2D Multiply(float s);
    Vec2D Div(float s);
    float Dot(Vec2D& v);
    void Normalize();    

	float x;
    float y;
};

enum class Axis {X_AXIS, Y_AXIS, Z_AXIS};

class Vec4D;

class Vec3D
{
public:
    Vec3D();
    Vec3D(float a, float b, float c);
    Vec3D(Vec4D& v);
    ~Vec3D();
    void print(std::string msg = "") { std::cout << msg << " " << x << ", " << y << ", " << z << std::endl; }

    void Rotate(Axis axis, float angle);
    float Magnitude();
    Vec3D Add(Vec3D& v);
    Vec3D Sub(Vec3D& v);
    Vec3D Multiply(float s);
    Vec3D Div(float s);
    Vec3D Cross(Vec3D& v);
    float Dot(Vec3D& v);
    void Normalize();

    float x;
    float y;
    float z;
};

class Vec4D
{
    public:
        Vec4D();
        Vec4D(float a, float b, float c, float d);
        Vec4D(Vec3D v);
        ~Vec4D();
        void print(std::string msg = "") { std::cout << msg << " " << x << ", " << y << ", " << z << std::endl; }


        float x;
        float y;
        float z;
        float w;
};
