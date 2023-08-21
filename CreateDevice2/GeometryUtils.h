#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include <cmath>

class Vector2
{
public:
    union {
        struct {
            float mX, mY;
        };
        float v[2];
    };
public:
    Vector2() : mX(0), mY(0) {}
    Vector2(float x, float y);
    Vector2(const Vector2& other);

    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    Vector2& operator=(const Vector2& other);

    float getX() const { return mX; }
    float getY() const { return mY; }

    void Normalize();
};

class Vector3
{
public:
    union {
        struct {
            float mX, mY, mZ;
        };
        float v[3];
    };
public:
    Vector3() : mX(0), mY(0), mZ(0) {}
    Vector3(float x, float y, float z);
    Vector3(const Vector3& other);

    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;
    Vector3& operator=(const Vector3& other);

    float getX() const { return mX; }
    float getY() const { return mY; }
    float getZ() const { return mZ; }

    void Normalize();
};

class Vector4
{
public:
    union {
        struct {
            float mX, mY, mZ, mW;
        };
        float v[4];
    };

    Vector4()
    {
        mX = mY = mZ = mW = 0.0f;
    }

    Vector4(float x, float y, float z, float w)
    {
        mX = x;
        mY = y;
        mZ = z;
        mW = w;
    }
};

class Matrix
{
public:
    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
public:
    Matrix() { Identity(); }
    ~Matrix() {}

    void Identity()
    {
        _11 = _12 = _13 = _14 = 0.0f;
        _21 = _22 = _23 = _24 = 0.0f;
        _31 = _32 = _33 = _34 = 0.0f;
        _41 = _42 = _43 = _44 = 0.0f;
        _11 = _22 = _33 = _44 = 1.0f;
    }

    Matrix operator * (Matrix const& matrix)
    {
        Matrix ret;
        for (int iColumn = 0; iColumn < 4; iColumn++)
        {
            for (int iRow = 0; iRow < 4; iRow++)
            {
                ret.m[iRow][iColumn] =
                    m[iRow][0] * matrix.m[0][iColumn] +
                    m[iRow][1] * matrix.m[1][iColumn] +
                    m[iRow][2] * matrix.m[2][iColumn] +
                    m[iRow][3] * matrix.m[3][iColumn];
            }
        }
        return ret;
    }
    Vector3 operator * (Vector3 const& v)
    {
        Vector4 ret;
        ret.mW = 1.0f;
        ret.mX = v.mX * _11 + v.mY * _21 + v.mZ * _31 + (float)1.0f * _41;
        ret.mY = v.mX * _12 + v.mY * _22 + v.mZ * _32 + (float)1.0f * _42;
        ret.mZ = v.mX * _13 + v.mY * _23 + v.mZ * _33 + (float)1.0f * _43;
        ret.mW = v.mX * _14 + v.mY * _24 + v.mZ * _34 + (float)1.0f * _44;

        /* ret.x /= ret.w;
         ret.y /= ret.w;
         ret.z /= ret.w;
         ret.w /= ret.w;*/

        return Vector3(ret.mX, ret.mY, ret.mZ);
    }
    float operator () (int iRow, int icol) const
    {
        return m[iRow][icol];
    }
    float& operator () (int iRow, int icol)
    {
        return m[iRow][icol];
    }

    void Translation(const Vector3& v)
    {
        _41 = v.mX;
        _42 = v.mY;
        _43 = v.mZ;
    }
    void Translation(float x, float y, float z)
    {
        _41 = x;
        _42 = y;
        _43 = z;
    }

    void Scale(const Vector3& v)
    {
        _11 = v.mX;
        _22 = v.mY;
        _33 = v.mZ;
    }
    void Scale(float x, float y, float z)
    {
        _11 = x;
        _22 = y;
        _33 = z;
    }

    void XRotate(float fRadian)
    {
        float fCos = cos(fRadian);
        float fSin = sin(fRadian);
        _22 = fCos;  _23 = fSin;
        _32 = -fSin;  _33 = fCos;
    }
    void YRotate(float fRadian)
    {
        float fCos = cos(fRadian);
        float fSin = sin(fRadian);
        _11 = fCos;  _13 = -fSin;
        _31 = fSin; _33 = fCos;
    }
    void ZRotate(float fRadian)
    {
        float fCos = cos(fRadian);
        float fSin = sin(fRadian);
        _11 = fCos;  _12 = fSin;
        _21 = -fSin; _22 = fCos;
    }
    Matrix Transpose()
    {
        Matrix ret;
        ret._11 = _11; ret._12 = _21; ret._13 = _31; ret._14 = _41;
        ret._21 = _12; ret._22 = _22; ret._23 = _32; ret._24 = _42;
        ret._31 = _13; ret._32 = _23; ret._33 = _33; ret._34 = _43;
        ret._41 = _14; ret._42 = _24; ret._43 = _34; ret._44 = _44;
        return ret;
    }
};

class Box {
public:
    Vector3 mFrontTopLeft;
    Vector3 mFrontTopRight;
    Vector3 mFrontBottomLeft;
    Vector3 mFrontBottomRight;
    Vector3 mBackTopLeft;
    Vector3 mBackTopRight;
    Vector3 mBackBottomLeft;
    Vector3 mBackBottomRight;
    float mWidth, mHeight, mDepth;

    Box() {
        mWidth = 0, mHeight = 0, mDepth = 0;
    };
    Box(const Vector3& frontTopLeft,
        const Vector3& backBottomRight,
        float width, float height, float depth);

    void Set(const Vector3& frontTopLeft,
        float width, float height, float depth) {
        Vector3 backBottomRight(frontTopLeft.mX + width, frontTopLeft.mY + height, frontTopLeft.mZ + depth);
        mFrontTopLeft = frontTopLeft;
        mFrontTopRight = Vector3(backBottomRight.getX(), frontTopLeft.getY(), frontTopLeft.getZ());
        mFrontBottomLeft = Vector3(frontTopLeft.getX(), backBottomRight.getY(), frontTopLeft.getZ());
        mFrontBottomRight = Vector3(backBottomRight.getX(), backBottomRight.getY(), frontTopLeft.getZ());
        mBackTopLeft = Vector3(frontTopLeft.getX(), frontTopLeft.getY(), backBottomRight.getZ());
        mBackTopRight = Vector3(backBottomRight.getX(), frontTopLeft.getY(), backBottomRight.getZ());
        mBackBottomLeft = Vector3(frontTopLeft.getX(), backBottomRight.getY(), backBottomRight.getZ());
        mBackBottomRight = backBottomRight;
        mWidth = width;
        mHeight = height;
        mDepth = depth;
    }
};

#endif