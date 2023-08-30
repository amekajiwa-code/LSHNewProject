#include "sample.h"

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
    mSound = SoundManager::GetInstance().Load(L"res/sound/song_bunker_1.ogg");
    mSound->Play();
    mEffectSound = SoundManager::GetInstance().Load(L"res/sound/slash_2.wav");
    #pragma endregion

    #pragma region 오브젝트세팅
    srand(time(NULL));
    mMapObj = new PlaneObject;
    mMapObj->Set(m_pDevice, m_pImmediateContext);
    mMapObj->SetScale(Vector3(static_cast<float>(g_dwWindowWidth), static_cast<float>(g_dwWindowHeight), 1.0f));
    mMapObj->Create(TextureManager::GetInstance(), L"res/background/LastStage.bmp", ShaderManager::GetInstance(), L"Plane.hlsl");

    mFloorObj = new PlaneObject;
    mFloorObj->Set(m_pDevice, m_pImmediateContext);
    mFloorObj->SetPos({ 0.0f, -static_cast<float>(g_dwWindowHeight) + 170.0f, 0.0f });
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
    mPlayer->SetScale(Vector3(50.0f, 50.0f, 75.0f));
    rt = { mPlayer->m_vPos.mX * 2.0f, mPlayer->m_vPos.mY * 2.0f };
    mPlayer->SetRect(rt, mPlayer->m_vScale.mX * 2.0f, mPlayer->m_vScale.mY * 2.0f);
    mPlayer->Create(TextureManager::GetInstance(), L"res/player/spr_idle/0.png", ShaderManager::GetInstance(), L"Plane.hlsl");
    #pragma endregion

    #pragma region 스프라이트애니메이션
    const Texture* tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/0.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/1.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/2.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/3.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/4.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/5.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/6.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/7.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/8.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/9.png");
    mPlayer->mIdleList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/10.png");
    mPlayer->mIdleList.push_back(tex);

    const Texture* tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/0.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/1.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/2.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/3.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/4.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/5.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/6.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/7.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/8.png");
    mPlayer->mRunList.push_back(tex1);
    tex1 = TextureManager::GetInstance().Load(L"res/player/spr_run/9.png");
    mPlayer->mRunList.push_back(tex1);

    const Texture* tex2 = TextureManager::GetInstance().Load(L"res/player/spr_jump/0.png");
    mPlayer->mJumpList.push_back(tex2);
    tex2 = TextureManager::GetInstance().Load(L"res/player/spr_jump/1.png");
    mPlayer->mJumpList.push_back(tex2);
    tex2 = TextureManager::GetInstance().Load(L"res/player/spr_jump/2.png");
    mPlayer->mJumpList.push_back(tex2);
    tex2 = TextureManager::GetInstance().Load(L"res/player/spr_jump/3.png");
    mPlayer->mJumpList.push_back(tex2);

    const Texture* tex3 = TextureManager::GetInstance().Load(L"res/player/spr_fall/0.png");
    mPlayer->mFallList.push_back(tex3);
    tex3 = TextureManager::GetInstance().Load(L"res/player/spr_fall/1.png");
    mPlayer->mFallList.push_back(tex3);
    tex3 = TextureManager::GetInstance().Load(L"res/player/spr_fall/2.png");
    mPlayer->mFallList.push_back(tex3);
    tex3 = TextureManager::GetInstance().Load(L"res/player/spr_fall/3.png");
    mPlayer->mFallList.push_back(tex3);

    const Texture* tex4 = TextureManager::GetInstance().Load(L"res/player/spr_attack/0.png");
    mPlayer->mAttackList.push_back(tex4);
    tex4 = TextureManager::GetInstance().Load(L"res/player/spr_attack/1.png");
    mPlayer->mAttackList.push_back(tex4);
    tex4 = TextureManager::GetInstance().Load(L"res/player/spr_attack/2.png");
    mPlayer->mAttackList.push_back(tex4);
    tex4 = TextureManager::GetInstance().Load(L"res/player/spr_attack/3.png");
    mPlayer->mAttackList.push_back(tex4);
    tex4 = TextureManager::GetInstance().Load(L"res/player/spr_attack/4.png");
    mPlayer->mAttackList.push_back(tex4);
    tex4 = TextureManager::GetInstance().Load(L"res/player/spr_attack/5.png");
    mPlayer->mAttackList.push_back(tex4);
    tex4 = TextureManager::GetInstance().Load(L"res/player/spr_attack/6.png");
    mPlayer->mAttackList.push_back(tex4);

    #pragma endregion
    
    #pragma region 카메라생성
    mMainCamera.Create(mPlayer->m_vPos, { static_cast<float>(m_dwWindowWidth), static_cast<float>(m_dwWindowHeight) });

    for (int iObj = 0; iObj < 5; iObj++)
    {
        Object* pObj = new Npc;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos(Vector3(randstep(-static_cast<float>(g_dwWindowWidth), +static_cast<float>(g_dwWindowWidth)),
            randstep(-static_cast<float>(g_dwWindowHeight), +static_cast<float>(g_dwWindowHeight)), 0));
        //pObj->SetPos(Vector3(0.0f, 0.0f, 0.0f));
        pObj->SetScale(Vector3(50.0f, 50.0f, 1.0f));
        Vector2 rt = { pObj->m_vPos.mX, pObj->m_vPos.mY };
        pObj->SetRect(rt, pObj->m_vScale.mX * 2.0f, pObj->m_vScale.mY * 2.0f);
        pObj->Create(TextureManager::GetInstance(), L"res/npc/anajuyo_alpha.png",
            ShaderManager::GetInstance(), L"Plane.hlsl");
        mNpcList.push_back(pObj);
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

    return true;
}
bool  sample::Frame()
{
    Vector3 curMouse = Input::GetInstance().GetWorldPos(
        { static_cast<float>(g_dwWindowWidth),
        static_cast<float>(g_dwWindowHeight) },
        mMainCamera.mCameraPos);
    mCursorObj->SetPos(curMouse);
    Vector2 rt = { mCursorObj->m_vPos.mX * 2.0f, mCursorObj->m_vPos.mY * 2.0f };
    mCursorObj->SetRect(rt, mCursorObj->m_vScale.mX * 2.0f, mCursorObj->m_vScale.mY * 2.0f);

    mPlayer->CheckCollision(mFloorObj);

    mPlayer->Frame();
    mMapObj->Frame();
    mFloorObj->Frame();
    mCursorObj->Frame();

    for (auto obj : mNpcList)
    {
        if (obj->m_bDead == false)
        {
            obj->Move(mGameTimer.mSecondPerFrame);
            obj->Frame();
            Vector2 rt = { obj->m_vPos.mX, obj->m_vPos.mY };
            obj->SetRect(rt, obj->m_vScale.mX * 2.0f, obj->m_vScale.mY * 2.0f);
        }
    }

    if (Input::GetInstance().mkeyState[VK_LBUTTON]
        == 2)
    {
        mEffectSound->PlayEffect();
        for (auto obj : mNpcList)
        {
            if (obj->m_bDead) continue;

            if ((curMouse.mX > obj->mRect.m_Min.mX && curMouse.mX < obj->mRect.m_Max.mX)
                && (curMouse.mY > obj->mRect.m_Min.mY && curMouse.mY < obj->mRect.m_Max.mY))
            {
                obj->m_bDead = true;
            }
        }
    }
    if (Input::GetInstance().mkeyState[VK_RBUTTON]
        == 2)
    {
        mSound->Stop();
    }
    if (Input::GetInstance().mkeyState[VK_SPACE]
        == 2)
    {
        mSound->Paused();
    }
    if (Input::GetInstance().mkeyState[VK_LEFT]
        == 3)
    {
        mSound->VolumeDown();
    }
    if (Input::GetInstance().mkeyState[VK_RIGHT]
        == 3)
    {
        mSound->VolumeUp();
    }

    return true;
}
bool  sample::Render()
{
    m_pImmediateContext->OMSetBlendState(mAlphaBlend, 0, -1);
    mMainCamera.mCameraPos = mPlayer->m_vPos;

    mMapObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mMapObj->Render();

    mFloorObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mFloorObj->Render();

    for (auto obj : mNpcList)
    {
        if (obj->m_bDead) continue;
        obj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
        obj->Render();
    }

    mPlayer->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mPlayer->PreRender();
    mPlayer->Render();
    mTexList.clear();
    mTexList = mPlayer->GetPlayerAnimation();
    if (!mTexList.empty())
    {
        mTexIndex = (int)(g_GameTimer * 10) % mTexList.size();
        if (mTexList[mTexIndex] != nullptr)
        {
            mTexList[mTexIndex]->Apply(m_pImmediateContext, 0);
        }
    }
    mPlayer->PostRender();

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

    //mSpriteObj->Release();
    //delete mSpriteObj;
    //mSpriteObj = nullptr;

    for (auto obj : mNpcList)
    {
        obj->Release();
        delete obj;
    }
    mNpcList.clear();
    mAlphaBlend->Release();
    return true;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) // 메인
{
    sample mySample;
    mySample.SetRegisterClassWindow(hInstance);
    mySample.SetWindow(L"아무거나", 1280, 720);
    ::ShowCursor(FALSE);
    mySample.Run();

    return 0;
}