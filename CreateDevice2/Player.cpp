#include "Player.h"
#include "Input.h"
#include "sample.h"

bool Player::Init()
{
	return false;
}

bool Player::Frame()
{
    if (Input::GetInstance().mkeyState['A'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mX -= 500.0f * g_SecondPerFrame;
    }
    if (Input::GetInstance().mkeyState['D'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mX += 500.0f * g_SecondPerFrame;
    }
    if (Input::GetInstance().mkeyState['W'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mY += 500.0f * g_SecondPerFrame;
    }
    if (Input::GetInstance().mkeyState['S'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mY -= 500.0f * g_SecondPerFrame;
    }

    Matrix matScale, matRotation, matTranslate;
    matScale.Scale(m_vScale);
    matRotation.ZRotate(m_vRotation.mZ);
    matTranslate.Translation(m_vPos);
    m_matWorld = matScale * matRotation * matTranslate;

	return true;
}

bool Player::Render()
{
    PlaneObject::Render();
	return false;
}

bool Player::Release()
{
	return false;
}