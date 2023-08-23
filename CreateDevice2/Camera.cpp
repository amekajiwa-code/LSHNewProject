#include "Camera.h"

bool Camera::Init()
{
    return false;
}

bool Camera::Frame()
{
    mMatView._41 = -mCameraPos.mX;
    mMatView._42 = -mCameraPos.mY;
    mMatView._43 = -mCameraPos.mZ;
    mMatOrthonormalProjection._11 = 2.0f / ((float)mWindowWidth);
    mMatOrthonormalProjection._22 = 2.0f / ((float)mWindowHeight);

    return true;
}

bool Camera::Render()
{
    return false;
}

bool Camera::Release()
{
    return false;
}

bool Camera::Create(Vector3 pos, Vector2 size)
{
    mCameraPos = pos;
    mWindowWidth = size.mX;
    mWindowHeight = size.mY;
    return false;
}

