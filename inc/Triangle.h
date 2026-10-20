#pragma once
#include "Vector.h"
#include <cstdint>
#include <stdint.h>
class Face
{
public:
    Face(int aa, int bb, int cc, uint32_t clr);
    int a;
    int b;
    int c;
    uint32_t color;
};

class Triangle
{
public:
	Triangle();
	~Triangle();

    Vec2D points[3];
    uint32_t color;
private:
	
};
	
