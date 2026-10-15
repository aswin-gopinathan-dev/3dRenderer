#pragma once
#include "Vector.h"
class Face
{
public:
    Face(int aa, int bb, int cc);
    int a;
    int b;
    int c;
};

class Triangle
{
public:
	Triangle();
	~Triangle();

    Vec2D points[3];
private:
	
};
	
