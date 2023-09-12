#include "Camera.h"

void Camera::SetView(XMFLOAT3 eye, XMFLOAT3 lookAt, XMFLOAT3 up)
{
    mViewMatrix = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));
}

void Camera::SetdProjection(float FOV, float aspectRatio, float nearest, float farDistance)
{
    float fovY = XMConvertToRadians(FOV);
    mProjMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearest, farDistance);
}

bool Camera::Init()
{
    return false;
}

bool Camera::Frame()
{

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

bool Camera::Create(XMFLOAT3 pos, XMFLOAT2 size)
{
    mCameraPos = pos;
    mWindowWidth = static_cast<DWORD>(size.x);
    mWindowHeight = static_cast<DWORD>(size.y);
    return true;
}

