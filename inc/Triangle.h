#pragma once
#include "Vector.h"
#include "Texture.h"
#include <cstdint>
#include <stdint.h>
class Face
{
public:
    Face(int aa, int bb, int cc, Texture aa_uv, Texture bb_uv, Texture cc_uv, uint32_t clr);
    int a;
    int b;
    int c;
    Texture a_uv;
    Texture b_uv;
    Texture c_uv;
    uint32_t color;
};

class Triangle
{
public:
	Triangle();
	~Triangle();

    Vec4D points[3];
    Texture tex[3];
    uint32_t color;
    double avg_depth;

private:
	
};
	
