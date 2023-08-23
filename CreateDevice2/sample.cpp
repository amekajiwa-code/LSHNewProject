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
    m_pMapObj->SetScale(Vector3(m_dwWindowWidth / 2, m_dwWindowHeight / 2, 1.0f));
    m_pMapObj->Create(m_texMgr, L"../../res/bg.jpg", m_shaderMgr, L"Plane.hlsl");

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
    m_matView._41 = -m_vCameraPos.mX;
    m_matView._42 = -m_vCameraPos.mY;
    m_matView._43 = -m_vCameraPos.mZ;
    m_matOrthoProjection._11 = 2.0f / ((float)m_dwWindowWidth);
    m_matOrthoProjection._22 = 2.0f / ((float)m_dwWindowHeight);

    m_pMapObj->SetMatrix(nullptr, &m_matView, &m_matOrthoProjection);
    m_pMapObj->Render();
    for (auto obj : m_NpcList)
    {
        obj->SetMatrix(nullptr, &m_matView, &m_matOrthoProjection);
        obj->Render();
    }
    return true;
}
bool  sample::Release()
{
    m_pMapObj->Release();
    delete m_pMapObj;
    m_pMapObj = nullptr;

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