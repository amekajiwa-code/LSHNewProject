#include "DebugCamera.h"
#include "Input.h"
#include "Timer.h"

bool  DebugCamera::Init()
{
	Camera::Init();
	return true;
}
bool  DebugCamera::Frame()
{
	if (Input::GetInstance().mkeyState[VK_SPACE] == static_cast<DWORD>(KeyState::KEY_DOWN))
	{
		m_fSpeed = 1000.0f;
	}
	if (Input::GetInstance().mkeyState[VK_SPACE] == static_cast<DWORD>(KeyState::KEY_UP))
	{
		m_fSpeed = 100.0f;
	}
	if (Input::GetInstance().mkeyState['W'] == static_cast<DWORD>(KeyState::KEY_HOLD))
	{
		XMVECTOR vOffset = mCameraPos;
		vOffset = m_vLook * Timer::GetInstance().mSecondPerFrame * m_fSpeed;
		mCameraPos = mCameraPos + vOffset;
	}
	if (Input::GetInstance().mkeyState['S'] == static_cast<DWORD>(KeyState::KEY_HOLD))
	{
		XMVECTOR vOffset = mCameraPos;
		vOffset = m_vLook * Timer::GetInstance().mSecondPerFrame * m_fSpeed;
		mCameraPos = mCameraPos - vOffset;
	}
	if (Input::GetInstance().mkeyState['A'] == static_cast<DWORD>(KeyState::KEY_HOLD))
	{
		XMVECTOR vOffset = mCameraPos;
		vOffset = m_vSide * Timer::GetInstance().mSecondPerFrame * m_fSpeed;
		mCameraPos = mCameraPos - vOffset;
	}
	if (Input::GetInstance().mkeyState['D'] == static_cast<DWORD>(KeyState::KEY_HOLD))
	{
		XMVECTOR vOffset = mCameraPos;
		vOffset = m_vSide * Timer::GetInstance().mSecondPerFrame * m_fSpeed;
		mCameraPos = mCameraPos + vOffset;
	}
	if (Input::GetInstance().mkeyState['Q'] == static_cast<DWORD>(KeyState::KEY_HOLD))
	{
		XMVECTOR vOffset = mCameraPos;
		vOffset = m_vUp * Timer::GetInstance().mSecondPerFrame * m_fSpeed;
		mCameraPos = mCameraPos + vOffset;
	}
	if (Input::GetInstance().mkeyState['E'] == static_cast<DWORD>(KeyState::KEY_HOLD))
	{
		XMVECTOR vOffset = mCameraPos;
		vOffset = m_vUp * Timer::GetInstance().mSecondPerFrame * m_fSpeed;
		mCameraPos = mCameraPos - vOffset;
	}
	if (Input::GetInstance().mkeyState[VK_LBUTTON] == static_cast<DWORD>(KeyState::KEY_HOLD))
	{
		float x = XMVectorGetX(Input::GetInstance().mOffset);
		float y = XMVectorGetY(Input::GetInstance().mOffset);
		m_fCameraYaw += XMConvertToRadians(x * 0.1f);
		m_fCameraPitch += XMConvertToRadians(y * 0.1f);
	}

	XMVECTOR gRotation;
	XMMATRIX matRotation;
	gRotation = XMQuaternionRotationRollPitchYaw(m_fCameraPitch, m_fCameraYaw, m_fCameraRoll); //pitch yaw roll
	matRotation = XMMatrixAffineTransformation(g_XMOne, g_XMZero, gRotation, mCameraPos);
	mViewMatrix = XMMatrixInverse(nullptr, matRotation);
	Camera::UpdateVector();

	//m_fCameraRoll += 1.0f * Timer::GetInstance().mSecondPerFrame;

	return true;
}
bool  DebugCamera::Render()
{
	Camera::Render();
	return true;
}
bool  DebugCamera::Release()
{
	Camera::Release();
	return true;
}