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
    //오브젝트 세팅
    srand(time(NULL));
    mMapObj = new PlaneObject;
    mMapObj->Set(m_pDevice, m_pImmediateContext);
    mMapObj->SetScale(Vector3(static_cast<float>(g_dwWindowWidth) / 2, static_cast<float>(g_dwWindowHeight) / 2, 1.0f));
    mMapObj->Create(TextureManager::GetInstance(), L"../../res/bg.jpg", ShaderManager::GetInstance(), L"Plane.hlsl");

    //스프라이트 애니메이션
    const Texture* tex = TextureManager::GetInstance().Load(L"../../res/anajuyo_ani_0.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"../../res/anajuyo_ani_1.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"../../res/anajuyo_ani_2.png");
    mTexList.push_back(tex);
    tex = TextureManager::GetInstance().Load(L"../../res/anajuyo_ani_3.png");
    mTexList.push_back(tex);

    mSpriteObj = new PlaneObject;
    mSpriteObj->Set(m_pDevice, m_pImmediateContext);
    mSpriteObj->SetPos({ 0.0f, 0.0f, 0.0f });
    mSpriteObj->SetScale(Vector3(100.0f, 100.0f, 1.0f));
    mSpriteObj->Create(TextureManager::GetInstance(), L"../../res/anajuyo_ani_0.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    mPlayer = new Player;
    mPlayer->Set(m_pDevice, m_pImmediateContext);
    mPlayer->SetPos({ 0.0f, 0.0f, 0.0f });
    mPlayer->SetScale(Vector3(50.0f, 50.0f, 1.0f));
    Vector2 rt = { mPlayer->m_vPos.mX * 2.0f, mPlayer->m_vPos.mY * 2.0f };
    mPlayer->SetRect(rt, mPlayer->m_vScale.mX * 2.0f, mPlayer->m_vScale.mY * 2.0f);
    mPlayer->Create(TextureManager::GetInstance(), L"../../res/siruyo.png", ShaderManager::GetInstance(), L"Plane.hlsl");

    
    //카메라 생성
    mMainCamera.Create(mPlayer->m_vPos, { static_cast<float>(m_dwWindowWidth), static_cast<float>(m_dwWindowHeight) });

    for (int iObj = 0; iObj < 10; iObj++)
    {
        Object* pObj = new Npc;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos(Vector3(randstep(-static_cast<float>(g_dwWindowWidth), +static_cast<float>(g_dwWindowWidth)),
            randstep(-static_cast<float>(g_dwWindowHeight), +static_cast<float>(g_dwWindowHeight)), 0));
        pObj->SetScale(Vector3(50.0f, 50.0f, 1.0f));
        Vector2 rt = { pObj->m_vPos.mX * 2.0f, pObj->m_vPos.mY * 2.0f };
        pObj->SetRect(rt, pObj->m_vScale.mX * 2.0f, pObj->m_vScale.mY * 2.0f);
        pObj->Create(TextureManager::GetInstance(), L"../../res/anajuyo_alpha.png",
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
            mFont.Create(pBackBuffer);
        }
        if (pBackBuffer) pBackBuffer->Release();
    }

    mFont.AddText(L"가", 10, 100, { 1.0f, 0.0f, 0.0f, 1.0f });
    mFont.AddText(L"나", 10, 150, { 0.0f, 1.0f, 0.0f, 1.0f });
    mFont.AddText(L"다", 10, 200, { 0.0f, 0.0f, 1.0f, 1.0f });

    

    return true;
}
bool  sample::Frame()
{
    if (Input::GetInstance().mkeyState[VK_HOME] == 2)
    {
        mTexIndex++;
        if (mTexIndex >= mTexList.size())
        {
            mTexIndex = mTexList.size() - 1;
        }
    }
    if (Input::GetInstance().mkeyState[VK_END] == 2)
    {
        mTexIndex--;
        if (mTexIndex < 0)
        {
            mTexIndex = 0;
        }
    }

    mPlayer->Frame();
    mMapObj->Frame();
    mSpriteObj->Frame();

    for (auto obj : mNpcList)
    {
        if (obj->m_bDead) continue;
        obj->Move(mGameTimer.mSecondPerFrame);
        obj->Frame();
    }

    for (auto obj : mNpcList)
    {
        if (mPlayer->mRect.ToRect(obj->mRect))
        {
            obj->m_bDead = true;
        }
    }

    return true;
}
bool  sample::Render()
{
    m_pImmediateContext->OMSetBlendState(mAlphaBlend, 0, -1);
    //mMainCamera.mCameraPos = mPlayer->m_vPos;

    mMapObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mMapObj->Render();

    for (auto obj : mNpcList)
    {
        if (obj->m_bDead == false)
        {
            obj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
            obj->Render();
        }
    }

    mPlayer->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mPlayer->Render();

    mSpriteObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mSpriteObj->PreRender();

    mTexIndex = (int)(g_GameTimer*8) % 4;
    if (mTexList[mTexIndex] != nullptr)
    {
        mTexList[mTexIndex]->Apply(m_pImmediateContext, 0);
    }

   mSpriteObj->PostRender();

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

    mSpriteObj->Release();
    delete mSpriteObj;
    mSpriteObj = nullptr;

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
    mySample.SetWindow(L"아무거나", 1600, 900);
    mySample.Run();

    return 0;
}