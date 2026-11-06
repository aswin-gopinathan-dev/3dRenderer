#pragma once
#include "Vector.h"

class Matrix
{
public:
	Matrix();
	~Matrix();

    float m[4][4];

    void Identity();
    void Scale(float sx, float sy, float sz);
    void Translate(float tx, float ty, float tz);
    void RotateX(float angle);
    void RotateY(float angle);
    void RotateZ(float angle);
    Matrix ProjectionMatrix(float fov, float aspect, float znear, float zfar);
    Vec4D Project(Vec4D v);

    Vec4D Multiply(Vec4D v);
    Matrix Multiply(Matrix& matrix);

private:
	
};
	
