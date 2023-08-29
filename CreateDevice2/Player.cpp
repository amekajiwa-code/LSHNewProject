#include "Player.h"
#include "Input.h"
#include "sample.h"

void Player::PlayerMove()
{
    //플레이어 조작
    if (Input::GetInstance().mkeyState['W'] == static_cast<DWORD>(KeyState::KEY_DOWN) && isFloor)
    {
        isJump = true;
        isFloor = false;
    }
    if (Input::GetInstance().mkeyState['S'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mY -= 500.0f * g_SecondPerFrame;
    }
    if (Input::GetInstance().mkeyState['A'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mX -= 500.0f * g_SecondPerFrame;
        isFlipY = true;
    }
    if (Input::GetInstance().mkeyState['D'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mX += 500.0f * g_SecondPerFrame;
        isFlipY = false;
    }

    if (isJump && (mJumpTimer <= 0.3f))
    {
        m_vPos.mY += 700.0f * g_SecondPerFrame;
        mJumpTimer += g_SecondPerFrame;
    }
    else
    {
        m_vPos.mY -= 300.0f * g_SecondPerFrame;
        isJump = false;
        mJumpTimer = 0.0f;
    }

    //카메라 이동
    if (m_vPos.mX < -static_cast<float>(g_dwWindowWidth))
    {
        m_vPos.mX = -static_cast<float>(g_dwWindowWidth);
    }
    if (m_vPos.mY <= -static_cast<float>(g_dwWindowHeight) + 225.0f)
    {
        isFloor = true;
        m_vPos.mY = -static_cast<float>(g_dwWindowHeight) + 225.0f;

    }
    if (m_vPos.mX > static_cast<float>(g_dwWindowWidth))
    {
        m_vPos.mX = static_cast<float>(g_dwWindowWidth);
    }
    if (m_vPos.mY > static_cast<float>(g_dwWindowHeight))
    {
        m_vPos.mY = static_cast<float>(g_dwWindowHeight);
    }
}

bool Player::Init()
{
    mJumpTimer = 0.0f;
	return true;
}

bool Player::Frame()
{
    PlayerMove();

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
    if (isFlipY)
    {
        m_VertexList.resize(6);
        m_VertexList[0].t.mX = 1.0f; m_VertexList[0].t.mY = 0.0f;
        m_VertexList[1].t.mX = 0.0f; m_VertexList[1].t.mY = 0.0f;
        m_VertexList[2].t.mX = 1.0f; m_VertexList[2].t.mY = 1.0f;
        m_VertexList[3].t.mX = 1.0f; m_VertexList[3].t.mY = 1.0f;
        m_VertexList[4].t.mX = 0.0f; m_VertexList[4].t.mY = 0.0f;
        m_VertexList[5].t.mX = 0.0f; m_VertexList[5].t.mY = 1.0f;
    }
    else
    {
        m_VertexList.resize(6);
        m_VertexList[0].t.mX = 0.0f; m_VertexList[0].t.mY = 0.0f;
        m_VertexList[1].t.mX = 1.0f; m_VertexList[1].t.mY = 0.0f;
        m_VertexList[2].t.mX = 0.0f; m_VertexList[2].t.mY = 1.0f;
        m_VertexList[3].t.mX = 0.0f; m_VertexList[3].t.mY = 1.0f;
        m_VertexList[4].t.mX = 1.0f; m_VertexList[4].t.mY = 0.0f;
        m_VertexList[5].t.mX = 1.0f; m_VertexList[5].t.mY = 1.0f;
    }

    PlaneObject::Render();
	return false;
}

bool Player::Release()
{
	return false;
}