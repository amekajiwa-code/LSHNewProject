#include "Camera.h"

bool Camera::Init()
{
    return false;
}

bool Camera::Frame()
{
    if (mCameraPos.mX < -static_cast<float>(g_dwWindowWidth / 2))
    {
        mCameraPos.mX = -static_cast<float>(g_dwWindowWidth / 2);
    }
    if (mCameraPos.mX > static_cast<float>(g_dwWindowWidth / 2))
    {
        mCameraPos.mX = static_cast<float>(g_dwWindowWidth / 2);
    }
    if (mCameraPos.mY > static_cast<float>(g_dwWindowHeight / 2))
    {
        mCameraPos.mY = static_cast<float>(g_dwWindowHeight / 2);
    }
    if (mCameraPos.mY < -static_cast<float>(g_dwWindowHeight / 2))
    {
        mCameraPos.mY = -static_cast<float>(g_dwWindowHeight / 2);
    }

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
    mWindowWidth = static_cast<DWORD>(size.mX);
    mWindowHeight = static_cast<DWORD>(size.mY);
    return false;
}

