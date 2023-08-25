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

	if (m_vPos.mX < -static_cast<float>(g_dwWindowWidth) / 2)
	{
		m_vPos.mX = -static_cast<float>(g_dwWindowWidth) / 2;
	}
	if (m_vPos.mY < -static_cast<float>(g_dwWindowHeight) / 2)
	{
		m_vPos.mY = -static_cast<float>(g_dwWindowHeight) / 2;
	}
	if (m_vPos.mX > static_cast<float>(g_dwWindowWidth) / 2)
	{
		m_vPos.mX = static_cast<float>(g_dwWindowWidth) / 2;
	}
	if (m_vPos.mY > static_cast<float>(g_dwWindowHeight) / 2)
	{
		m_vPos.mY = static_cast<float>(g_dwWindowHeight) / 2;
	}

    Vector2 rt = { m_vPos.mX, m_vPos.mY };
    SetRect(rt, m_vScale.mX * 2.0f, m_vScale.mY * 2.0f);

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