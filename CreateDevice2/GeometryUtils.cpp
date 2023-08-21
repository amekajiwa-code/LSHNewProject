#include "GeometryUtils.h"

Vector2::Vector2(float x, float y) : mX(x), mY(y) {}

Vector2::Vector2(const Vector2& other) : mX(other.mX), mY(other.mY) {}

bool Vector2::operator==(const Vector2& other) const {
    return (mX == other.mX) && (mY == other.mY);
}

bool Vector2::operator!=(const Vector2& other) const {
    return !(*this == other);
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(mX + other.mX, mY + other.mY);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(mX - other.mX, mY - other.mY);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(mX * scalar, mY * scalar);
}

Vector2 Vector2::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector2(mX / scalar, mY / scalar);
    }

    return *this;
}

Vector2& Vector2::operator=(const Vector2& other) {
    if (this != &other) {
        mX = other.mX;
        mY = other.mY;
    }
    return *this;
}

void Vector2::Normalize() {
    double length = std::sqrt(mX * mX + mY * mY);
    if (length != 0.0f) {
        mX /= length;
        mY /= length; 
    }
}
//------------------------------------------------------------------------------------------
Vector3::Vector3(float x, float y, float z) : mX(x), mY(y), mZ(z) {}

Vector3::Vector3(const Vector3& other) : mX(other.mX), mY(other.mY), mZ(other.mZ) {}

bool Vector3::operator==(const Vector3& other) const {
    return (mX == other.mX) && (mY == other.mY) && (mZ == other.mZ);
}

bool Vector3::operator!=(const Vector3& other) const {
    return !(*this == other);
}

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(mX + other.mX, mY + other.mY, mZ + other.mZ);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(mX - other.mX, mY - other.mY, mZ - other.mZ);
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(mX * scalar, mY * scalar, mZ * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector3(mX / scalar, mY / scalar, mZ / scalar);
    }

    return *this;
}

Vector3& Vector3::operator=(const Vector3& other) {
    if (this != &other) {
        mX = other.mX;
        mY = other.mY;
        mZ = other.mZ;
    }
    return *this;
}

void Vector3::Normalize() {
    double length = std::sqrt(mX * mX + mY * mY + mZ * mZ);
    if (length != 0.0f) {
        mX /= length;
        mY /= length;
        mZ /= length;
    }
}

Box::Box(const Vector3& frontTopLeft,
    const Vector3& backBottomRight,
    float width, float height, float depth)
    : mFrontTopLeft(frontTopLeft), mWidth(width), mHeight(height), mDepth(depth) {
    mFrontTopRight = Vector3(frontTopLeft.getX() + width, frontTopLeft.getY(), frontTopLeft.getZ());
    mFrontBottomLeft = Vector3(frontTopLeft.getX(), frontTopLeft.getY() + height, frontTopLeft.getZ());
    mFrontBottomRight = Vector3(frontTopLeft.getX() + width, frontTopLeft.getY() + height, frontTopLeft.getZ());
    mBackTopLeft = Vector3(frontTopLeft.getX(), frontTopLeft.getY(), frontTopLeft.getZ() + depth);
    mBackTopRight = Vector3(frontTopLeft.getX() + width, frontTopLeft.getY(), frontTopLeft.getZ() + depth);
    mBackBottomLeft = Vector3(frontTopLeft.getX(), frontTopLeft.getY() + height, frontTopLeft.getZ() + depth);
    mBackBottomRight = Vector3(mBackTopLeft.getX() + width, mBackTopLeft.getY() + height, mBackTopLeft.getZ() + depth);
}