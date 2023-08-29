#include "sample.h"

bool  sample::Init()
{
    // alpha blending
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

    //사운드
    mSound = SoundManager::GetInstance().Load(L"res/sound/song_bunker_1.ogg");
    mSound->Play();
    mEffectSound = SoundManager::GetInstance().Load(L"res/sound/slash_2.wav");
    //오브젝트 세팅
    srand(time(NULL));
    mMapObj = new PlaneObject;
    mMapObj->Set(m_pDevice, m_pImmediateContext);
    mMapObj->SetScale(Vector3(static_cast<float>(g_dwWindowWidth), static_cast<float>(g_dwWindowHeight), 1.0f));
    mMapObj->Create(TextureManager::GetInstance(), L"res/background/LastStage.bmp", ShaderManager::GetInstance(), L"Plane.hlsl");

    //스프라이트 애니메이션
    const Texture* tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/0.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/1.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/2.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/3.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/4.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/5.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/6.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/7.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/8.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/9.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"res/player/spr_idle/10.png");
    mTexList.push_back(tex);

    /*mSpriteObj = new PlaneObject;
    mSpriteObj->Set(m_pDevice, m_pImmediateContext);
    mSpriteObj->SetPos({ (static_cast<float>(g_dwWindowWidth) / 2) - 100.0f,
        +(static_cast<float>(g_dwWindowHeight) / 2) - 100.0f,
        0.0f });
    mSpriteObj->SetScale(Vector3(100.0f, 100.0f, 1.0f));
    mSpriteObj->Create(TextureManager::GetInstance(), L"../../res/animation/spr_idle/0.png", ShaderManager::GetInstance(), L"Plane.hlsl");*/

    mCursorObj = new PlaneObject;
    mCursorObj->Set(m_pDevice, m_pImmediateContext);
    mCursorObj->SetPos({ 0.0f, 0.0f, 0.0f });
    mCursorObj->SetScale(Vector3(25.0f, 25.0f, 1.0f));
    mCursorObj->Create(TextureManager::GetInstance(), L"res/ui/cursor_0.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    mPlayer = new Player;
    mPlayer->Set(m_pDevice, m_pImmediateContext);
    mPlayer->SetPos({ 0.0f, 0.0f, 0.0f });
    mPlayer->SetScale(Vector3(50.0f, 50.0f, 75.0f));
    Vector2 rt = { mPlayer->m_vPos.mX * 2.0f, mPlayer->m_vPos.mY * 2.0f };
    mPlayer->SetRect(rt, mPlayer->m_vScale.mX * 2.0f, mPlayer->m_vScale.mY * 2.0f);
    mPlayer->Create(TextureManager::GetInstance(), L"res/player/spr_idle/0.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    
    //카메라 생성
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
    // 폰트
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

    //Writer::GetInstance().AddText(L"프레임", 20, 20, { 1.0f, 0.0f, 0.0f, 1.0f });

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

    mPlayer->Frame();
    mMapObj->Frame();
    //mSpriteObj->Frame();
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

    for (auto obj : mNpcList)
    {
        if (obj->m_bDead) continue;
        obj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
        obj->Render();
    }

    mPlayer->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mPlayer->PreRender();

    //mSpriteObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    //mSpriteObj->PreRender();

    mTexIndex = (int)(g_GameTimer * 10) % mTexList.size();
    if (mTexList[mTexIndex] != nullptr)
    {
        mTexList[mTexIndex]->Apply(m_pImmediateContext, 0);
    }

   //mSpriteObj->PostRender();
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