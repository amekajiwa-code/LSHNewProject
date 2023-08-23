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
    m_pDevice->CreateBlendState(&bsd, &m_AlphaBlend);

    m_texMgr.Set(m_pDevice, m_pImmediateContext);
    m_shaderMgr.Set(m_pDevice, m_pImmediateContext);

    srand(time(NULL));
    m_pMapObj = new PlaneObject;
    m_pMapObj->Set(m_pDevice, m_pImmediateContext);
    m_pMapObj->SetScale(Vector3(static_cast<float>(m_dwWindowWidth) / 2, static_cast<float>(m_dwWindowHeight) / 2, 1.0f));
    m_pMapObj->Create(m_texMgr, L"../../res/bg.jpg", m_shaderMgr, L"Plane.hlsl");

    mPlayer = new Player;
    mPlayer->Set(m_pDevice, m_pImmediateContext);
    mPlayer->SetPos({ 0.0f, 0.0f, 0.0f });
    mPlayer->SetScale(Vector3(50.0f, 50.0f, 1.0f));
    mPlayer->Create(m_texMgr, L"../../res/anajuyo_alpha.png", m_shaderMgr, L"Plane.hlsl");

    mMainCamera.Create(mPlayer->m_vPos, { static_cast<float>(m_dwWindowWidth), static_cast<float>(m_dwWindowHeight) });

    for (int iObj = 0; iObj < 10; iObj++)
    {
        Object* pObj = new Npc;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos(Vector3(randstep(-1000.0f, +1000.0f),
            randstep(-1000.0f, +1000.0f), 0));
        pObj->SetScale(Vector3(50.0f, 50.0f, 1.0f));
        pObj->Create(m_texMgr, L"../../res/anajuyo_alpha.png",
            m_shaderMgr, L"Plane.hlsl");
        m_NpcList.push_back(pObj);
    }
    return true;
}
bool  sample::Frame()
{
    mPlayer->Frame();
    m_pMapObj->Frame();

    for (auto obj : m_NpcList)
    {
        obj->Move(m_GameTimer.m_fSecondPerFrame);
        obj->Frame();
    }
    return true;
}
bool  sample::Render()
{
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);
    mMainCamera.mCameraPos = mPlayer->m_vPos;
    m_pMapObj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    m_pMapObj->Render();

    for (auto obj : m_NpcList)
    {
        obj->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
        obj->Render();
    }

    mPlayer->SetMatrix(nullptr, &mMainCamera.mMatView, &mMainCamera.mMatOrthonormalProjection);
    mPlayer->Render();

    return true;
}
bool  sample::Release()
{
    m_pMapObj->Release();
    delete m_pMapObj;
    m_pMapObj = nullptr;

    mPlayer->Release();
    delete mPlayer;
    mPlayer = nullptr;

    for (auto obj : m_NpcList)
    {
        obj->Release();
        delete obj;
    }
    m_NpcList.clear();
    m_AlphaBlend->Release();
    return true;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) // 메인
{
    /*sample mySample;
    mySample.SetRegisterClassWindow(hInstance);
    mySample.SetWindow(L"아무거나", 800, 600);
    mySample.Run();*/

    sample& mySample = sample::GetInstance();
    mySample.SetRegisterClassWindow(hInstance);
    mySample.SetWindow(L"아무거나", 800, 600);
    mySample.Run();

    return 0;
}