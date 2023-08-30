#include "Player.h"
#include "Input.h"
#include "Writer.h"

void Player::PlayerMove()
{
    //플레이어 조작
    if (Input::GetInstance().mkeyState['W'] == static_cast<DWORD>(KeyState::KEY_DOWN) && isFloor)
    {
        isJump = true;
    }
    /*if (Input::GetInstance().mkeyState['S'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mY -= 500.0f * g_SecondPerFrame;
    }*/
    if (Input::GetInstance().mkeyState['A'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mX -= 500.0f * g_SecondPerFrame;
        isFlipY = true;
        mPlayerState = PlayerState::RUN;
    }
    if (Input::GetInstance().mkeyState['D'] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        m_vPos.mX += 500.0f * g_SecondPerFrame;
        isFlipY = false;
        mPlayerState = PlayerState::RUN;
    }

    if (isJump && (mJumpTimer <= MAX_JUMP_TIME))
    {
        m_vPos.mY += 700.0f * g_SecondPerFrame;
        mJumpTimer += g_SecondPerFrame;
        mPlayerState = PlayerState::JUMP;
    }
    else
    {
        if (isFloor == false)
        {
            m_vPos.mY -= 300.0f * g_SecondPerFrame;
            mJumpTimer = 0.0f;
            isJump = false;
            mPlayerState = PlayerState::FALL;
        }
    }

    //카메라 이동
    if (m_vPos.mX < -static_cast<float>(g_dwWindowWidth))
    {
        m_vPos.mX = -static_cast<float>(g_dwWindowWidth);
    }
    if (m_vPos.mY <= -static_cast<float>(g_dwWindowHeight))
    {
        m_vPos.mY = -static_cast<float>(g_dwWindowHeight);
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

void Player::PlayerAttack()
{
    //sdfsdf
}

bool Player::CheckCollision(Object* other)
{
    if (Player::mRect.ToRect(other->mRect))
    {
        if (other->mTag == "Floor") isFloor = true;
        mPlayerState = PlayerState::IDLE;
    }
    else
    {
        isFloor = false;
    }
    return true;
}

vector<const Texture*> Player::GetPlayerAnimation()
{
    switch (mPlayerState)
    {
    case PlayerState::IDLE:
        return mIdleList;
        break;
    case PlayerState::RUN:
        return mRunList;
        break;
    case PlayerState::JUMP:
        return mJumpList;
        break;
    case PlayerState::FALL:
        return mFallList;
        break;
    case PlayerState::ATTACK:
        return mAttackList;
        break;
    default:
        break;
    }
}

bool Player::Init()
{
	return true;
}

bool Player::Frame()
{
    PlayerMove();
#ifdef _DEBUG
    wstring fontFPS;
    switch (mPlayerState)
    {
    case PlayerState::IDLE:
        fontFPS = L"State : Idle";
        break;
    case PlayerState::RUN:
        fontFPS = L"State : Run";
        break;
    case PlayerState::JUMP:
        fontFPS = L"State : Jump";
        break;
    case PlayerState::FALL:
        fontFPS = L"State : Fall";
        break;
    case PlayerState::ATTACK:
        fontFPS = L"State : Attack";
        break;
    default:
        fontFPS = L"State : ";
        break;
    }
    Writer::GetInstance().AddText(fontFPS, 10, 100, { 1.0f, 1.0f, 1.0f, 1.0f });
#endif
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
        m_VertexList[0].t.mX = 1.0f; m_VertexList[0].t.mY = 0.0f;
        m_VertexList[1].t.mX = 0.0f; m_VertexList[1].t.mY = 0.0f;
        m_VertexList[2].t.mX = 1.0f; m_VertexList[2].t.mY = 1.0f;
        m_VertexList[3].t.mX = 1.0f; m_VertexList[3].t.mY = 1.0f;
        m_VertexList[4].t.mX = 0.0f; m_VertexList[4].t.mY = 0.0f;
        m_VertexList[5].t.mX = 0.0f; m_VertexList[5].t.mY = 1.0f;
    }
    else
    {
        m_VertexList[0].t.mX = 0.0f; m_VertexList[0].t.mY = 0.0f;
        m_VertexList[1].t.mX = 1.0f; m_VertexList[1].t.mY = 0.0f;
        m_VertexList[2].t.mX = 0.0f; m_VertexList[2].t.mY = 1.0f;
        m_VertexList[3].t.mX = 0.0f; m_VertexList[3].t.mY = 1.0f;
        m_VertexList[4].t.mX = 1.0f; m_VertexList[4].t.mY = 0.0f;
        m_VertexList[5].t.mX = 1.0f; m_VertexList[5].t.mY = 1.0f;
    }
    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_VertexList.at(0), 0, 0);
	return true;
}

bool Player::Release()
{
	return false;
}