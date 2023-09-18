#include "Core.h"

bool  Core::Init() { return true; }
bool  Core::Frame() { return true; }
bool  Core::Render() { return true; }
bool  Core::Release() { return true; }

bool  Core::EngineInit()
{
    Device::Init();
    CreateDepthStencilState();
    Timer::GetInstance().Init();
    Input::GetInstance().Init();
    Writer::GetInstance().Init();
    SoundManager::GetInstance().Init();
    mMainCamera->Init();
    Init();

    return true;
}

bool  Core::EngineFrame()
{
    Timer::GetInstance().Frame();
    Input::GetInstance().Frame();
    Writer::GetInstance().Frame();
    SoundManager::GetInstance().Frame();
    Device::Frame();
    mMainCamera->Frame();
    Frame();

    return true;
}

bool  Core::EngineRender()
{
    Device::PreRender();
    m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
    Render();
    Timer::GetInstance().Render();
    Input::GetInstance().Render();
    Writer::GetInstance().Render();
    mMainCamera->Render();
    Device::PostRender();

    return true;
}

bool  Core::EngineRelease()
{
    Release();
    Timer::GetInstance().Release();
    Input::GetInstance().Release();
    Writer::GetInstance().Release();
    SoundManager::GetInstance().Release();
    mMainCamera->Release();
    Device::Release();

    return true;
}
bool Core::Run()
{
    EngineInit();
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (!EngineFrame() || !EngineRender())
            {
                break;
            }
        }
    }
    EngineRelease();
    return true;
}

void Core::CreateDepthStencilState()
{
    HRESULT hr;
    // 깊이버퍼 상태값 세팅
    D3D11_DEPTH_STENCIL_DESC dsDescDepth;
    ZeroMemory(&dsDescDepth, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsDescDepth.DepthEnable = TRUE;
    dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    // 해당 픽셀의 깊이 연산을 하여 출력여부를 판단한다.
    // 지형의 깊이 값 >=  깊이버퍼의 픽셀값(1.0f) 
    dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    //Stencil
    dsDescDepth.StencilEnable = FALSE;
    dsDescDepth.StencilReadMask = 1;
    dsDescDepth.StencilWriteMask = 1;
    dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
    dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    // 디폴트 값
    dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    if (FAILED(hr = m_pDevice->CreateDepthStencilState(&dsDescDepth,
        m_pDepthStencilState.GetAddressOf())))
    {
        return;
    }
    dsDescDepth.DepthEnable = FALSE;
    if (FAILED(hr = m_pDevice->CreateDepthStencilState(&dsDescDepth,
        m_pDepthStencilStateDisable.GetAddressOf())))
    {
        return;
    }
    return;
}