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
    float length = std::sqrt(mX * mX + mY * mY);
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
    float length = std::sqrt(mX * mX + mY * mY + mZ * mZ);
    if (length != 0.0f) {
        mX /= length;
        mY /= length;
        mZ /= length;
    }
}

bool Vector4::operator==(const Vector4& other) const {
    return (mX == other.mX) && (mY == other.mY) && (mZ == other.mZ) && (mW == other.mW);
}

bool Vector4::operator!=(const Vector4& other) const {
    return !(*this == other);
}

Vector4 Vector4::operator+(const Vector4& other) const {
    return Vector4(mX + other.mX, mY + other.mY, mZ + other.mZ, mW + other.mW);
}

Vector4 Vector4::operator-(const Vector4& other) const {
    return Vector4(mX - other.mX, mY - other.mY, mZ - other.mZ, mW - other.mW);
}

Vector4 Vector4::operator*(float scalar) const {
    return Vector4(mX * scalar, mY * scalar, mZ * scalar, mW * scalar);
}

Vector4 Vector4::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector4(mX / scalar, mY / scalar, mZ / scalar, mW / scalar);
    }

    return *this;
}

Vector4& Vector4::operator=(const Vector4& other) {
    if (this != &other) {
        mX = other.mX;
        mY = other.mY;
        mZ = other.mZ;
        mW = other.mW;
    }
    return *this;
}

void Vector4::Normalize() {
    float length = std::sqrt(mX * mX + mY * mY + mZ * mZ + mW * mW);
    if (length != 0.0f) {
        mX /= length;
        mY /= length;
        mZ /= length;
        mW /= length;
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

bool Rect::operator == (Rect& p)
{
    if (fabs(v.mX - p.v.mX) > 0.0001f)
    {
        if (fabs(v.mY - p.v.mY) > 0.0001f)
        {
            if (fabs(m_fWidth - p.m_fWidth) > 0.0001f)
            {
                if (fabs(m_fHeight - p.m_fHeight) > 0.0001f)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Rect::operator != (Rect& p)
{
    return !(*this == p);
}

Rect Rect::operator + (Rect& p)
{
    Rect rt;
    float fMinX = std::min(m_Min.mX, p.m_Min.mX);
    float fMinY = std::min(m_Min.mY, p.m_Min.mY);
    float fMaxX = std::max(m_Max.mX, p.m_Max.mX);
    float fMaxY = std::max(m_Max.mY, p.m_Max.mY);
    Vector2 center = { (fMinX + fMaxX) * 0.5f, (fMinY + fMaxY) * 0.5f };
    rt.Set(center, fMaxX - fMinX, fMaxY - fMinY);
    return rt;
}

Rect Rect::operator - (Rect& p)
{
    Rect rt;
    rt.m_bEnable = false;
    if (ToRect(p))
    {
        //left, top          right
        //      bottom
        float fx = (m_Min.mX > p.m_Min.mX) ? m_Min.mX : p.m_Min.mX;
        float fy = (m_Min.mY > p.m_Min.mY) ? m_Min.mY : p.m_Min.mY;
        float right = (m_Max.mX < p.m_Max.mX) ? m_Max.mX : p.m_Max.mX;
        float bottom = (m_Max.mY < p.m_Max.mY) ? m_Max.mY : p.m_Max.mY;
        Vector2 center = { (fx + right) * 0.5f, (fy + bottom) * 0.5f };
        rt.Set(center, right - fx, bottom - fy);
        rt.m_bEnable = true;
    }
    return rt;
}

Rect Rect::operator - (Vector2& p)
{
    v.mX -= p.mX;
    v.mY -= p.mY;
    return Rect(v.mX, v.mY, m_fWidth, m_fHeight);
}

Rect Rect::operator * (float fValue)
{
    if (fValue <= 0.0f)
    {
        return *this;
    }
    m_fWidth *= fValue;
    m_fHeight *= fValue;
    return Rect(v.mX, v.mY, m_fWidth, m_fHeight);
}

Rect Rect::operator / (float fValue)
{
    if (fValue <= 0.0f)
    {
        return *this;
    }
    m_fWidth /= fValue;
    m_fHeight /= fValue;
    return Rect(v.mX, v.mY, m_fWidth, m_fHeight);
}

void Rect::Set(Vector2 p)
{
    m_Center = p;
    v = { p.mX, p.mY };
    s = { m_fWidth, m_fHeight };
    Set(m_fWidth, m_fHeight);
}

void Rect::Set(float fw, float fh)
{
    m_fWidth = fw;
    m_fHeight = fh;
    m_Half = { m_fWidth * 0.5f,m_fHeight * 0.5f };
    m_Point[0] = { v.mX, v.mY };
    m_Point[1] = { v.mX + m_fWidth, v.mY };
    m_Point[2] = { v.mX + m_fWidth, v.mY - m_fHeight };
    m_Point[3] = { v.mX, v.mY - m_fHeight };
    m_Max = m_Point[1];
    m_Min = m_Point[3];
}

void Rect::Set(Vector2 p, float fw, float fh)
{
    m_Center = p;
    v = { p.mX - fw / 2.0f, p.mY + fh / 2.0f };
    s = { fw, fh };
    Set(fw, fh);
}

void Rect::Set(float fx, float fy, float fw, float fh)
{
    m_Center.mX = fx;
    m_Center.mY = fy;
    s = { fw, fh };
    v = { fx - fw / 2.0f, fy + fh / 2.0f };

    Set(fw, fh);
}

bool Rect::ToRect(Rect& rt)
{
    Rect sum = (*this) + rt;
    float fX = m_fWidth + rt.m_fWidth;
    float fY = m_fHeight + rt.m_fHeight;
    if (sum.m_fWidth <= fX)
    {
        if (sum.m_fHeight <= fY)
        {
            return true;
        }
    }
    return false;
}

bool Rect::ToPoint(Vector2& p)
{
    if (m_Min.mX <= p.mX && m_Max.mX >= p.mX &&
        m_Min.mY <= p.mY && m_Max.mY >= p.mY)
    {
        return true;
    }
    return false;
}

Rect::Rect() : m_bEnable(true) {}

Rect::Rect(float fx, float fy, float fw, float fh)
{
    m_bEnable = true;
    Set(fx, fy, fw, fh);
}

//Vector2 OBB::GetHeightVec(Rect rt)
//{
//    Vector2 heightVec = { 0,0 };
//
//
//
//    heightVec.mX = rt.m_fHeight * cos(DegreeToRadian(rt.angle - 90)) * 0.5f;
//    heightVec.mY = rt.m_fHeight * -sin(DegreeToRadian(rt.angle - 90)) * 0.5f;
//
//
//
//    return heightVec;
//}
//
//
//
//Vector2 OBB::GetWidthVec(Rect rt)
//{
//    Vector2 widthVec = { 0,0 };
//
//
//
//    widthVec.x = rt.size.x * cos(DegreeToRadian(rt.angle)) * 0.5f;
//    widthVec.y = rt.size.x * sin(DegreeToRadian(rt.angle)) * 0.5f;
//
//
//
//    return widthVec;
//}
//
//
//
//bool OBB::RectToPoint(Vector2& point, Rect& rect)
//{
//    if (rect.min.x <= point.x && rect.max.x >= point.x
//        && rect.min.y <= point.y && rect.max.y >= point.y)
//        return true;
//
//
//
//    return false;
//}
//
//
//
//bool OBB::RectToRect(Rect& rect1, Rect& rect2)
//{
//    Vector2 distanceVec = rect2.center - rect1.center;
//    Vector2 vec[4] = { GetHeightVec(rect1),GetHeightVec(rect2),GetWidthVec(rect1),GetWidthVec(rect2) };
//    Vector2 unit;
//    float sum;
//
//
//
//    for (int i = 0; i < 4; ++i)
//    {
//        sum = 0;
//        unit = vec[i].GetNomalVector();
//
//
//
//        for (int j = 0; j < 4; ++j)
//        {
//            float b = vec[j] | unit;
//            sum += fabs(vec[j] | unit);
//        }
//
//
//
//        float a = distanceVec | unit;
//
//
//
//        if (fabs(distanceVec | unit) > sum)
//            return false;
//    }
//
//
//
//    return true;
//}