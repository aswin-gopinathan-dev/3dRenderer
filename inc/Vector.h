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

	float x;
    float y;
};

enum class Axis {X_AXIS, Y_AXIS, Z_AXIS};

class Vec3D
{
public:
    Vec3D();
    Vec3D(float a, float b, float c);
    ~Vec3D();
    void print(std::string msg = "") { std::cout << msg << " " << x << ", " << y << ", " << z << std::endl; }

    void Rotate(Axis axis, float angle);

    float x;
    float y;
    float z;
};
