#pragma once

class Vec2D
{
public:
	Vec2D();
    Vec2D(float a, float b);
	~Vec2D();

	float x;
    float y;
};


class Vec3D
{
public:
    Vec3D();
    Vec3D(float a, float b, float c);
    ~Vec3D();

    float x;
    float y;
    float z;
};
