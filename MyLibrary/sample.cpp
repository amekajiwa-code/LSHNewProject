#include "sample.h"

void sample::CameraViberation()
{
    float shakeMagnitude = 50.0f; // 흔들림 강도

    // 무작위한 흔들림 벡터 생성
    float shakeX = randstep(-shakeMagnitude, shakeMagnitude);
    float shakeY = randstep(-shakeMagnitude, shakeMagnitude);

    // 현재 카메라 위치에 흔들림 벡터를 더하여 새로운 카메라 위치 설정
    mMainCamera.mCameraPos = {
        mPlayer->m_vPos.mX + shakeX,
        mPlayer->m_vPos.mY + shakeY,
        0
    };
}

bool  sample::Init()
{
    #pragma region 알파블랜딩
    D3D11_BLEND_DESC bsd;
    ZeroMemory(&bsd, sizeof(bsd));
    bsd.RenderTarget[0].BlendEnable = true;
    bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pDevice->CreateBlendState(&bsd, &mAlphaBlend);

    TextureManager::GetInstance().Set(m_pDevice, m_pImmediateContext);
    ShaderManager::GetInstance().Set(m_pDevice, m_pImmediateContext);
    #pragma endregion

    #pragma region 사운드
    mSound = SoundManager::GetInstance().Load(L"res/sound/song_savageTerminal.wav");
    mSound->Play(true);
    mSound->VolumeDown();
    Sound* slash = SoundManager::GetInstance().Load(L"res/sound/death_sword.wav");
    SoundManager::GetInstance().playListMap.insert(make_pair("slash", slash));
    Sound* playerDeath = SoundManager::GetInstance().Load(L"res/sound/playerdie.wav");
    SoundManager::GetInstance().playListMap.insert(make_pair("playerDeath", playerDeath));
    Sound* rewindSound = SoundManager::GetInstance().Load(L"res/sound/Rewind.wav");
    SoundManager::GetInstance().playListMap.insert(make_pair("rewindSound", rewindSound));
    #pragma endregion

    #pragma region 오브젝트세팅
    srand(time(NULL));
    mMapObj = new PlaneObject;
    mMapObj->Set(m_pDevice, m_pImmediateContext);
    mMapObj->SetScale(Vector3(static_cast<float>(g_dwWindowWidth), static_cast<float>(g_dwWindowHeight), 1.0f));
    mMapObj->Create(TextureManager::GetInstance(), L"res/background/maple_bg_1.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    mFloorObj = new PlaneObject;
    mFloorObj->Set(m_pDevice, m_pImmediateContext);
    mFloorObj->SetPos({ 0.0f, -static_cast<float>(g_dwWindowHeight) + 250.0f, 0.0f });
    mFloorObj->SetScale(Vector3(static_cast<float>(g_dwWindowWidth), 20.0f, 1.0f));
    Vector2 rt = { mFloorObj->m_vPos.mX, mFloorObj->m_vPos.mY};
    mFloorObj->SetRect(rt, mFloorObj->m_vScale.mX * 2.0f, mFloorObj->m_vScale.mY * 2.0f);
    mFloorObj->SetTag("Floor");
    mFloorObj->Create(TextureManager::GetInstance(), L"res/floor.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    mCursorObj = new PlaneObject;
    mCursorObj->Set(m_pDevice, m_pImmediateContext);
    mCursorObj->SetPos({ 0.0f, 0.0f, 0.0f });
    mCursorObj->SetScale(Vector3(25.0f, 25.0f, 1.0f));
    mCursorObj->Create(TextureManager::GetInstance(), L"res/ui/cursor_0.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    mPlayer = new Player;
    mPlayer->Set(m_pDevice, m_pImmediateContext);
    mPlayer->SetPos({ 0.0f, 0.0f, 0.0f });
    mPlayer->SetScale(Vector3(36.0f, 35.0f, 1.0f));
    rt = { mPlayer->m_vPos.mX * 2.0f, mPlayer->m_vPos.mY * 2.0f };
    mPlayer->SetRect(rt, mPlayer->m_vScale.mX * 2.0f, mPlayer->m_vScale.mY * 2.0f);
    mPlayer->Create(TextureManager::GetInstance(), L"res/player/spr_idle/idle_0.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    mEffectObj = new PlaneObject;
    mEffectObj->Set(m_pDevice, m_pImmediateContext);
    mEffectObj->SetPos({ 0.0f, 0.0f, 0.0f });
    mEffectObj->SetScale(Vector3(123.0f, 30.0f, 1.0f));
    mEffectObj->Create(TextureManager::GetInstance(), L"res/effect/spr_master_slash/master_slash_0.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    for (int i = 0; i < 5; ++i)
    {
        Npc* pObj = new Npc;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos({ randstep(-1000, 0), -static_cast<float>(g_dwWindowHeight) + 290.0f, 0.0f });
        pObj->SetScale(Vector3(30.0f, 36.0f, 1.0f));
        rt = { pObj->m_vPos.mX, pObj->m_vPos.mY };
        pObj->SetRect(rt, pObj->m_vScale.mX * 2.0f, pObj->m_vScale.mY * 2.0f);
        pObj->Create(TextureManager::GetInstance(), L"res/npc/Grunt/spr_grunt_idle/grunt_idle_0.png",
            ShaderManager::GetInstance(), L"Plane.hlsl");
        pObj->SetTarget(mPlayer);
        mNpcList.push_back(pObj);

        Object* mNpcEffectObj = new PlaneObject;
        mNpcEffectObj->Set(m_pDevice, m_pImmediateContext);
        mNpcEffectObj->SetPos({ 0.0f, 0.0f, 0.0f });
        mNpcEffectObj->SetScale(Vector3(64.0f, 64.0f, 1.0f));
        mNpcEffectObj->Create(TextureManager::GetInstance(), L"res/effect/spr_gruntslash/gruntslash_0.png", ShaderManager::GetInstance(), L"Plane.hlsl");
        mNpcEffectObjList.push_back(mNpcEffectObj);
    } 

    for (int i = 0; i < 5; ++i)
    {
        Gunman* pObj = new Gunman;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos({ randstep(0, 1000), -static_cast<float>(g_dwWindowHeight) + 290.0f, 0.0f });
        pObj->SetScale(Vector3(49.0f, 50.0f, 1.0f));
        rt = { pObj->m_vPos.mX, pObj->m_vPos.mY };
        pObj->SetRect(rt, pObj->m_vScale.mX * 2.0f, pObj->m_vScale.mY * 2.0f);
        pObj->Create(TextureManager::GetInstance(), L"res/npc/Gangster/spr_gangsteridle/gangidle_0.png",
            ShaderManager::GetInstance(), L"Plane.hlsl");
        pObj->SetTarget(mPlayer);
        mGunmanList.push_back(pObj);
    }
    
    #pragma endregion

    #pragma region 플레이어_애니메이션
    const Texture* tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_0.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_1.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_2.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_3.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_4.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_5.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_6.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_7.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_8.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_9.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/idle_10.png");
    mPlayer->AddAnimationList(PlayerState::IDLE, tex);

    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_0.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_1.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_2.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_3.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_4.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_5.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_6.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_7.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_8.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_run/run_9.png");
    mPlayer->AddAnimationList(PlayerState::RUN, tex);

    tex = TextureManager::GetInstance().Load(L"res/player/spr_jump/jump_0.png");
    mPlayer->AddAnimationList(PlayerState::JUMP, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_jump/jump_1.png");
    mPlayer->AddAnimationList(PlayerState::JUMP, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_jump/jump_2.png");
    mPlayer->AddAnimationList(PlayerState::JUMP, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_jump/jump_3.png");
    mPlayer->AddAnimationList(PlayerState::JUMP, tex);

    tex = TextureManager::GetInstance().Load(L"res/player/spr_fall/fall_0.png");
    mPlayer->AddAnimationList(PlayerState::FALL, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_fall/fall_1.png");
    mPlayer->AddAnimationList(PlayerState::FALL, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_fall/fall_2.png");
    mPlayer->AddAnimationList(PlayerState::FALL, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_fall/fall_3.png");
    mPlayer->AddAnimationList(PlayerState::FALL, tex);

    tex = TextureManager::GetInstance().Load(L"res/player/spr_attack/attack_0.png");
    mPlayer->AddAnimationList(PlayerState::ATTACK, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_attack/attack_1.png");
    mPlayer->AddAnimationList(PlayerState::ATTACK, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_attack/attack_2.png");
    mPlayer->AddAnimationList(PlayerState::ATTACK, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_attack/attack_3.png");
    mPlayer->AddAnimationList(PlayerState::ATTACK, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_attack/attack_4.png");
    mPlayer->AddAnimationList(PlayerState::ATTACK, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_attack/attack_5.png");
    mPlayer->AddAnimationList(PlayerState::ATTACK, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_attack/attack_6.png");
    mPlayer->AddAnimationList(PlayerState::ATTACK, tex);

    tex = TextureManager::GetInstance().Load(L"res/player/spr_ground/ground_0.png");
    mPlayer->AddAnimationList(PlayerState::DEAD, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_ground/ground_1.png");
    mPlayer->AddAnimationList(PlayerState::DEAD, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_ground/ground_2.png");
    mPlayer->AddAnimationList(PlayerState::DEAD, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_ground/ground_3.png");
    mPlayer->AddAnimationList(PlayerState::DEAD, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_ground/ground_4.png");
    mPlayer->AddAnimationList(PlayerState::DEAD, tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_ground/ground_5.png");
    mPlayer->AddAnimationList(PlayerState::DEAD, tex);
    #pragma endregion
    
    #pragma region 이펙트_애니메이션
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_slash/slash_0.png");
    mEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_slash/slash_1.png");
    mEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_slash/slash_2.png");
    mEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_slash/slash_3.png");
    mEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_slash/slash_4.png");
    mEffectList.push_back(tex);

    tex = TextureManager::GetInstance().Load(L"res/effect/spr_gruntslash/gruntslash_0.png");
    mNpcEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_gruntslash/gruntslash_1.png");
    mNpcEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_gruntslash/gruntslash_2.png");
    mNpcEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_gruntslash/gruntslash_3.png");
    mNpcEffectList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/effect/spr_gruntslash/gruntslash_4.png");
    mNpcEffectList.push_back(tex);
    #pragma endregion

    #pragma region NPC_애니메이션
    for (Npc* npc : mNpcList)
    {
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_0.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_1.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_2.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_3.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_4.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_5.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_6.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_idle/grunt_idle_7.png");
        npc->AddAnimationList(NpcState::IDLE, tex);

        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_0.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_1.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_2.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_3.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_4.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_5.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_6.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_7.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_8.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_run/grunt_run_9.png");
        npc->AddAnimationList(NpcState::RUN, tex);

        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_0.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_1.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_2.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_3.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_4.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_5.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_6.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_attack/grunt_attack_7.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);

        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_0.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_1.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_2.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_3.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_4.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_5.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_6.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_7.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_8.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_9.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_10.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_11.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_12.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_13.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_14.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Grunt/spr_grunt_hurtground/grunt_hurtground_15.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
    }
#pragma endregion

    #pragma region NPC_애니메이션_갱스터
    for (Npc* npc : mGunmanList)
    {
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_0.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_1.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_2.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_3.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_4.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_5.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_6.png");
        npc->AddAnimationList(NpcState::IDLE, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsteridle/gangidle_7.png");
        npc->AddAnimationList(NpcState::IDLE, tex);

        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_0.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_1.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_2.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_3.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_4.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_5.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_6.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_7.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_8.png");
        npc->AddAnimationList(NpcState::RUN, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterrun/gangrun_9.png");
        npc->AddAnimationList(NpcState::RUN, tex);

        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterwhip/gangwhip_0.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterwhip/gangwhip_1.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterwhip/gangwhip_2.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterwhip/gangwhip_3.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterwhip/gangwhip_4.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterwhip/gangwhip_5.png");
        npc->AddAnimationList(NpcState::ATTACK, tex);

        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_0.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_1.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_2.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_3.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_4.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_5.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_6.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_7.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_8.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_9.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_10.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_11.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_12.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
        tex = TextureManager::GetInstance().Load(L"res/npc/Gangster/spr_gangsterhurtground/ganghurt_13.png");
        npc->AddAnimationList(NpcState::DEAD, tex);
    }
#pragma endregion

    #pragma region 폰트
    HRESULT hr;

    if (m_pSwapChain)
    {
        IDXGISurface1* pBackBuffer;
        hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1),
            (LPVOID*)&pBackBuffer);
        if (SUCCEEDED(hr))
        {
            Writer::GetInstance().Create(pBackBuffer);
        }
        if (pBackBuffer) pBackBuffer->Release();
    }
    #pragma endregion

    //카메라생성
    mMainCamera.Create(mPlayer->m_vPos, { static_cast<float>(m_dwWindowWidth), static_cast<float>(m_dwWindowHeight) });
    //게임매니저 세팅
    GameManager::GetInstance().player = mPlayer;
    GameManager::GetInstance().npcList = mNpcList;
    GameManager::GetInstance().gunmanList = mGunmanList;

    return true;
}
bool  sample::Frame()
{
    curMouse = Input::GetInstance().GetWorldPos(
        { static_cast<float>(g_dwWindowWidth),
        static_cast<float>(g_dwWindowHeight) },
        mMainCamera.mCameraPos);
    mCursorObj->SetPos(curMouse);

    Vector2 rt = { mCursorObj->m_vPos.mX * 2.0f, mCursorObj->m_vPos.mY * 2.0f };
    mCursorObj->SetRect(rt, mCursorObj->m_vScale.mX * 2.0f, mCursorObj->m_vScale.mY * 2.0f);

    mEffectObj->SetPos(mPlayer->m_vPos);
    rt = { mEffectObj->m_vPos.mX, mEffectObj->m_vPos.mY };
    mEffectObj->SetRect(rt, mEffectObj->m_vScale.mX * 2.0f, mEffectObj->m_vScale.mY * 2.0f);

    mPlayer->Frame();
    mMapObj->Frame();
    mFloorObj->Frame();
    mCursorObj->Frame();
    mEffectObj->Frame();

    for (auto obj : mNpcList)
    {
        obj->Frame();
        if (obj->m_bDead == false)
        {
            Vector2 rt = { obj->m_vPos.mX, obj->m_vPos.mY };
            obj->SetRect(rt, obj->m_vScale.mX, obj->m_vScale.mY);
        }
    }

    for (int i = 0; i < mNpcList.size(); ++i)
    {
        Vector3 newPos = mNpcList.at(i)->m_vPos;
        newPos.mX += 10.0f;
        mNpcEffectObjList.at(i)->SetPos(newPos);
        /*Vector2 rt = { mNpcEffectObjList.at(i)->m_vPos.mX, mNpcEffectObjList.at(i)->m_vPos.mY };
        mNpcEffectObjList.at(i)->SetRect(rt, mNpcEffectObjList.at(i)->m_vScale.mX * 2.0f, mNpcEffectObjList.at(i)->m_vScale.mY * 2.0f);*/
        mNpcEffectObjList.at(i)->Frame();
    }

    for (auto obj : mGunmanList)
    {
        obj->Frame();
        if (obj->m_bDead == false)
        {
            Vector2 rt = { obj->m_vPos.mX, obj->m_vPos.mY };
            obj->SetRect(rt, obj->m_vScale.mX, obj->m_vScale.mY);
        }
    }

    if ((Input::GetInstance().mkeyState[VK_LBUTTON] == 2) &&
        (mPlayer->GetPlayerState() != PlayerState::ATTACK) &&
        (mPlayer->GetPlayerState() != PlayerState::DEAD) &&
        (GameManager::GetInstance().isReplay == false))
    {
        mPlayer->mEffectSound->PlayEffect();
        mPlayer->SetPlayerState(PlayerState::ATTACK);

        // 바닥 각도 (0도)를 기준으로 한 curMouse의 각도 계산
        float deltaX = curMouse.mX - mMainCamera.mCameraPos.mX; // X 좌표 차이
        float deltaY = curMouse.mY - mMainCamera.mCameraPos.mY; // Y 좌표 차이
        float angleToMouse = atan2(deltaY, deltaX); // 라디안 단위로 각도 계산
        //mEffectObj->mRect.SetAngle(RadianToDegree(angleToMouse));
        if (GameManager::GetInstance().isRewind == false || GameManager::GetInstance().isReplay == false)
        {
            mPlayer->SetRadianAngle(angleToMouse);
        }

        Input::GetInstance().curWorldPos = curMouse;
    }


    if (mPlayer->GetPlayerState() == PlayerState::ATTACK)
    {
        for (auto obj : mNpcList)
        {
            if (obj->m_bDead) continue;

            if (obb.RectToRect(mEffectObj->mRect, obj->mRect) && (obj->GetInvincible() == false))
            {
                obj->m_bDead = true;
                obj->SetNPCState(NpcState::DEAD);
                obj->SetScale({56.0f, 41.0f, 1.0f});
                SoundManager::GetInstance().playListMap.find("slash")->second->Play(false);
            }
        }

        for (auto obj : mGunmanList)
        {
            if (obj->m_bDead) continue;

            if (obb.RectToRect(mEffectObj->mRect, obj->mRect) && (obj->GetInvincible() == false))
            {
                obj->m_bDead = true;
                obj->SetNPCState(NpcState::DEAD);
                obj->SetScale({ 44.0f, 34.0f, 1.0f });
                SoundManager::GetInstance().playListMap.find("slash")->second->Play(false);
            }
        }
    }

    mPlayer->CheckCollision(mFloorObj);
    return true;
}
bool  sample::Render()
{
    m_pImmediateContext->OMSetBlendState(mAlphaBlend, 0, -1);
    mMainCamera.mCameraPos = mPlayer->m_vPos;

    mMapObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mMapObj->Render();

    mFloorObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    //mFloorObj->Render();

    for (auto obj : mNpcList)
    {
        obj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
        obj->Render();
    }

    for (auto obj : mGunmanList)
    {
        obj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
        obj->Render();
    }

    #pragma region 플레이어_렌더
    mPlayer->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mPlayer->Render();
    #pragma endregion

    #pragma region 이펙트_렌더
    if (mPlayer->GetPlayerState() == PlayerState::ATTACK)
    {
        m_pImmediateContext->UpdateSubresource(mEffectObj->m_pVertexBuffer, 0, nullptr, &mEffectObj->m_VertexList.at(0), 0, 0);
        mEffectObj->m_matWorld.ZRotate(mPlayer->GetRadianAngle());
        //mEffectObj->m_matWorld.ZRotate(DegreeToRadian(mEffectObj->mRect.mAngle));
        
        mEffectObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
        mEffectObj->PreRender();

        if (!mEffectList.empty())
        {
            mEffectIndex = (int)(g_GameTimer * 10 * Timer::GetInstance().mTimeScale) % mEffectList.size();
            if (mEffectList[mEffectIndex] != nullptr)
            {
                mEffectList[mEffectIndex]->Apply(m_pImmediateContext, 0);
            }
        }
        mEffectObj->PostRender();
    }

    for (int i = 0; i < mNpcList.size(); ++i)
    {
        if (mNpcList.at(i)->GetNPCState() == NpcState::ATTACK)
        {
            float range = mNpcEffectObjList.at(i)->m_vPos.mX - mPlayer->m_vPos.mX;

            if (range > 0)
            {
                mNpcEffectObjList.at(i)->m_VertexList[0].t.mX = 1.0f; mNpcEffectObjList.at(i)->m_VertexList[0].t.mY = 0.0f;
                mNpcEffectObjList.at(i)->m_VertexList[1].t.mX = 0.0f; mNpcEffectObjList.at(i)->m_VertexList[1].t.mY = 0.0f;
                mNpcEffectObjList.at(i)->m_VertexList[2].t.mX = 1.0f; mNpcEffectObjList.at(i)->m_VertexList[2].t.mY = 1.0f;
                mNpcEffectObjList.at(i)->m_VertexList[3].t.mX = 1.0f; mNpcEffectObjList.at(i)->m_VertexList[3].t.mY = 1.0f;
                mNpcEffectObjList.at(i)->m_VertexList[4].t.mX = 0.0f; mNpcEffectObjList.at(i)->m_VertexList[4].t.mY = 0.0f;
                mNpcEffectObjList.at(i)->m_VertexList[5].t.mX = 0.0f; mNpcEffectObjList.at(i)->m_VertexList[5].t.mY = 1.0f;
            }
            else
            {
                mNpcEffectObjList.at(i)->m_VertexList[0].t.mX = 0.0f; mNpcEffectObjList.at(i)->m_VertexList[0].t.mY = 0.0f;
                mNpcEffectObjList.at(i)->m_VertexList[1].t.mX = 1.0f; mNpcEffectObjList.at(i)->m_VertexList[1].t.mY = 0.0f;
                mNpcEffectObjList.at(i)->m_VertexList[2].t.mX = 0.0f; mNpcEffectObjList.at(i)->m_VertexList[2].t.mY = 1.0f;
                mNpcEffectObjList.at(i)->m_VertexList[3].t.mX = 0.0f; mNpcEffectObjList.at(i)->m_VertexList[3].t.mY = 1.0f;
                mNpcEffectObjList.at(i)->m_VertexList[4].t.mX = 1.0f; mNpcEffectObjList.at(i)->m_VertexList[4].t.mY = 0.0f;
                mNpcEffectObjList.at(i)->m_VertexList[5].t.mX = 1.0f; mNpcEffectObjList.at(i)->m_VertexList[5].t.mY = 1.0f;
            }

            m_pImmediateContext->UpdateSubresource(mNpcEffectObjList.at(i)->m_pVertexBuffer, 0, nullptr, &mNpcEffectObjList.at(i)->m_VertexList.at(0), 0, 0);
            mNpcEffectObjList.at(i)->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
            mNpcEffectObjList.at(i)->PreRender();

            if (!mNpcEffectList.empty())
            {
                mNpcEffectIndex = (int)(g_GameTimer * 5 * Timer::GetInstance().mTimeScale) % mNpcEffectList.size();
                if (mNpcEffectList[mNpcEffectIndex] != nullptr)
                {
                    mNpcEffectList[mNpcEffectIndex]->Apply(m_pImmediateContext, 0);
                }
            }
            mNpcEffectObjList.at(i)->PostRender();
        }
    }
    
    #pragma endregion

   mCursorObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
   mCursorObj->Render();

    return true;
}
bool  sample::Release()
{
    mMapObj->Release();
    delete mMapObj;
    mMapObj = nullptr;

    mPlayer->Release();
    delete mPlayer;
    mPlayer = nullptr;
    
    for (auto obj : mNpcList)
    {
        obj->Release();
        delete obj;
    }
    mNpcList.clear();
    mAlphaBlend->Release();
    return true;
}

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) // 메인
{
    sample mySample;
    mySample.SetRegisterClassWindow(hInstance);
    mySample.SetWindow(L"아무거나", 1280, 720);
    ::ShowCursor(FALSE);
    mySample.Run();

    return 0;
}