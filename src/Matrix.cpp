#include "../inc/Matrix.h"
#include <math.h>

Matrix::Matrix()
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            m[i][j] = 0.0f;
        }
    }

    Identity();
}

Matrix::~Matrix()
{
}

void Matrix::Identity()
{
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
}

void Matrix::Scale(float sx, float sy, float sz)
{
    m[0][0] = sx;
    m[1][1] = sy;
    m[2][2] = sz;
    m[3][3] = 1.0f;
}

void Matrix::Translate(float tx, float ty, float tz)
{
    m[0][3] = tx;
    m[1][3] = ty;
    m[2][3] = tz;
    m[3][3] = 1.0f;
}

void Matrix::RotateX(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    m[1][1] = c;
    m[1][2] = -s;
    m[2][1] = s;
    m[2][2] = c;
}

void Matrix::RotateY(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    m[0][0] = c;
    m[0][2] = s;
    m[2][0] = -s;
    m[2][2] = c;
}

void Matrix::RotateZ(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    m[0][0] = c;
    m[0][1] = -s;
    m[1][0] = s;
    m[1][1] = c;
}

Vec4D Matrix::Multiply(Vec4D v)
{
    Vec4D result;
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

    return result;
}

Matrix Matrix::Multiply(Matrix& matrix)
{
    Matrix result;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            result.m[i][j] = m[i][0] * matrix.m[0][j] + m[i][1] * matrix.m[1][j] + m[i][2] * matrix.m[2][j] + m[i][3] * matrix.m[3][j];
        }
    }

    return result;
}


