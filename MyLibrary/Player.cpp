#include "Player.h"
#include "Input.h"
#include "Writer.h"
#include "Timer.h"
#include "GameManager.h"
#include <stack>

void Player::MoveX(bool isFlipY)
{
    mPlayerState = PlayerState::RUN;

    if (isFlipY)
    {
        m_vPos.mX -= mSpeed * g_SecondPerFrame;
    }
    else
    {
        m_vPos.mX += mSpeed * g_SecondPerFrame;
    }

    if (isFloor && !isDashSound) {
        mDashSound->Play(false);
        isDashSound = true;
    }
    else if (mDashTimer < MAX_Dash_TIME)
    {
        if (isFlipY)
        {
            m_vPos.mX -= (mSpeed * 0.3f) * g_SecondPerFrame;
        }
        else
        {
            m_vPos.mX += (mSpeed * 0.3f) * g_SecondPerFrame;
        }
        mDashTimer += g_SecondPerFrame;
    }
    else if (isFloor)
    {
        mRunSound->Play(false);
    }  
}

void Player::PlayerMove()
{
    //플레이어 조작
    if (GameManager::GetInstance().isReplay == false)
    {
        if (Input::GetInstance().mkeyState['W'] == static_cast<DWORD>(KeyState::KEY_DOWN) && isFloor)
        {
            mJumpSound->Play(false);
            isJump = true;
            isLandSound = false;
        }
        if (Input::GetInstance().mkeyState['S'] == static_cast<DWORD>(KeyState::KEY_HOLD) && !isFloor)
        {
            m_vPos.mY -= 800.0f * g_SecondPerFrame;
        }
        if (Input::GetInstance().mkeyState['A'] == static_cast<DWORD>(KeyState::KEY_HOLD))
        {
            isFlipY = true;
            MoveX(isFlipY);
        }
        if (Input::GetInstance().mkeyState['D'] == static_cast<DWORD>(KeyState::KEY_HOLD))
        {
            isFlipY = false;
            MoveX(isFlipY);
        }
        if (Input::GetInstance().mkeyState['A'] == static_cast<DWORD>(KeyState::KEY_UP) ||
            Input::GetInstance().mkeyState['D'] == static_cast<DWORD>(KeyState::KEY_UP))
        {
            mPlayerState = PlayerState::IDLE;
            isDashSound = false;
            mDashTimer = 0.0f;
        }
    }

    if (isJump && (mJumpTimer <= MAX_JUMP_TIME))
    {
        m_vPos.mY += (mPower * 0.7f) * g_SecondPerFrame;
        mJumpTimer += g_SecondPerFrame;
        mPlayerState = PlayerState::JUMP;
    }
    else if (isFloor == false)
    {
        m_vPos.mY -= mGrabity * g_SecondPerFrame;
        mJumpTimer = 0.0f;
        isJump = false;
        mPlayerState = PlayerState::FALL;
    }
    else if (!isLandSound)
    {
        mLandSound->VolumeDown();
        mLandSound->Play(false);
        isLandSound = true;
    }
    else if (mPlayerState != PlayerState::RUN)
    {
        mPlayerState = PlayerState::IDLE;
    }

    //카메라 이동
    if (m_vPos.mX < -static_cast<float>(g_dwWindowWidth) + 300.0f)
    {
        m_vPos.mX = -static_cast<float>(g_dwWindowWidth) + 300.0f;
    }
    if (m_vPos.mY <= -static_cast<float>(g_dwWindowHeight))
    {
        m_vPos.mY = -static_cast<float>(g_dwWindowHeight);
    }
    if (m_vPos.mX > static_cast<float>(g_dwWindowWidth) - 300.0f)
    {
        m_vPos.mX = static_cast<float>(g_dwWindowWidth) - 300.0f;
    }
    if (m_vPos.mY > static_cast<float>(g_dwWindowHeight))
    {
        m_vPos.mY = static_cast<float>(g_dwWindowHeight);
    }
}

void Player::PlayerAttack()
{
    if (mAttackTimer <= MAX_ATTACK_TIME)
    {
        mTargetDirection = Input::GetInstance().curWorldPos - m_vPos;

        Vector3 NormalX;
        mTargetDirection.Normalize();

        if (mTargetDirection.mX < 0)
        {
            isFlipY = true;
            NormalX = { -1.0f, 0.0f, 0.0f };
        }
        else
        {
            isFlipY = false;
            NormalX = { 1.0f, 0.0f, 0.0f };
        }

        if (isFloor && mTargetDirection.mY < 0.0f)
        {
            mVelocity = NormalX * mPower * g_SecondPerFrame;
        }
        else if (PlaneObject::m_vPos.mY > (lastFloorY + 150.0f))
        {
            mVelocity = mTargetDirection * (mPower / 3) * g_SecondPerFrame;
        }
        else
        {
            mVelocity = mTargetDirection * mPower * g_SecondPerFrame;
        }

        m_vPos = m_vPos + mVelocity;
        mAttackTimer += g_SecondPerFrame;
    }
    else if (mDelayTimer <= MAX_DELAY_TIME)
    {

        if (!isFloor) m_vPos = m_vPos + (mVelocity / 2);
        mDelayTimer += g_SecondPerFrame;
    }
    else
    {
        mAttackTimer = 0.0f;
        mDelayTimer = 0.0f;
        mPlayerState = PlayerState::IDLE;
        return;
    }
 }

bool Player::CheckCollision(Object* other)
{
    if (Player::mRect.ToRect(other->mRect) && other->mTag == "Floor")
    {
        isFloor = true;
        lastFloorY = other->m_vPos.mY;
        return true;
    }
    else
    {
        isFloor = false;
        return false;
    }
}

void Player::AddAnimationList(PlayerState state, const Texture* texture)
{
    mAniList.push_back(make_pair(state, texture));
}

vector<const Texture*> Player::GetAnimationList(PlayerState state)
{
    vector<const Texture*> texList;

    for (auto it = mAniList.begin(); it != mAniList.end(); ++it)
    {
        if (it->first == state) {
            texList.push_back(it->second);
        }
    }

    return texList;
}

bool Player::Init()
{
	return true;
}

bool Player::Frame()
{
    if (PlaneObject::m_bDead)
    {
        mPlayerState = PlayerState::DEAD;

        auto it = SoundManager::GetInstance().playListMap.find("playerDeath");
        if (it != SoundManager::GetInstance().playListMap.end())
        {
            it->second->Play(false);
            SoundManager::GetInstance().playListMap.erase(it);
        }
    }
    else
    {
        Sound* playerDeath = SoundManager::GetInstance().Load(L"res/sound/playerdie.wav");
        SoundManager::GetInstance().playListMap.insert(make_pair("playerDeath", playerDeath));
    }

    switch (mPlayerState)
    {
    case PlayerState::IDLE:
        PlayerMove();
        PlaneObject::SetScale({ 36.0f, 35.0f, 1.0f });
        maxAniTime = 0.2f;
        break;
    case PlayerState::RUN:
        PlayerMove();
        PlaneObject::SetScale({ 44.0f, 32.0f, 1.0f });
        maxAniTime = 0.1f;
        break;
    case PlayerState::JUMP:
        PlayerMove();
        PlaneObject::SetScale({ 32.0f, 42.0f, 1.0f });
        maxAniTime = 0.1f;
        break;
    case PlayerState::FALL:
        PlayerMove();
        PlaneObject::SetScale({ 42.0f, 48.0f, 1.0f });
        maxAniTime = 0.1f;
        break;
    case PlayerState::ATTACK:
        PlayerAttack();
        PlaneObject::SetScale({ 62.0f, 42.0f, 1.0f });
        maxAniTime = 0.1f;
        break;
    case PlayerState::DEAD:
        PlaneObject::SetScale({ 57.0f, 25.0f, 1.0f });
        maxAniTime = 0.2f;
        if (isFloor == false)
        {
            m_vPos.mY -= mGrabity * g_SecondPerFrame;
        }
    default:
        break;
    }

    if (Input::GetInstance().mkeyState[VK_SHIFT] == static_cast<DWORD>(KeyState::KEY_HOLD))
    {
        Timer::GetInstance().mTimeScale = 0.25f;
    }
    if (Input::GetInstance().mkeyState[VK_SHIFT] == static_cast<DWORD>(KeyState::KEY_UP))
    {
        Timer::GetInstance().mTimeScale = 1.0f;
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
    PlaneObject::PreRender();

    vector<const Texture*> texList = GetAnimationList(mPlayerState);
    if (!texList.empty())
    {
        if (mAniTimer < maxAniTime)
        {
            mAniTimer += Timer::GetInstance().mSecondPerFrame;
        }
        else
        {
            ++mAniCount;
            mAniTimer = 0.0f;
        }

        if (mAniCount >= texList.size() && mPlayerState == PlayerState::DEAD)
        {
            --mAniCount;
        }

        if (mAniCount >= texList.size())
        {
            mAniCount = 0;
        }

        texList[mAniCount]->Apply(m_pImmediateContext, 0);
    }
    

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

    PlaneObject::PostRender();
	return true;
}

bool Player::Release()
{
	return false;
}