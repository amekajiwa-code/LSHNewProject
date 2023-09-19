#include "Camera.h"

void Camera::UpdateVector()
{
    float elementX = XMVectorGetZ(mViewMatrix.r[0]);
    float elementY = XMVectorGetZ(mViewMatrix.r[1]);
    float elementZ = XMVectorGetZ(mViewMatrix.r[2]);
    m_vLook = XMVectorSet(elementX, elementY, elementZ, 0.0f);

    elementX = XMVectorGetY(mViewMatrix.r[0]);
    elementY = XMVectorGetY(mViewMatrix.r[1]);
    elementZ = XMVectorGetY(mViewMatrix.r[2]);
    m_vUp = XMVectorSet(elementX, elementY, elementZ, 0.0f);

    elementX = XMVectorGetX(mViewMatrix.r[0]);
    elementY = XMVectorGetX(mViewMatrix.r[1]);
    elementZ = XMVectorGetX(mViewMatrix.r[2]);
    m_vSide = XMVectorSet(elementX, elementY, elementZ, 0.0f);    
}

void Camera::SetView(XMFLOAT3 eye, XMFLOAT3 lookAt, XMFLOAT3 up)
{
    mViewMatrix = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));
}

void Camera::SetProjection(float FOV, float aspectRatio, float nearest, float farDistance)
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
    mCameraPos = XMLoadFloat3(&pos);
    mWindowWidth = static_cast<DWORD>(size.x);
    mWindowHeight = static_cast<DWORD>(size.y);
    return true;
}

