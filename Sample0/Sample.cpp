#include "Sample.h"

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

#pragma region 맵오브젝트 생성
    MapDesc mapDesc = {
        9,
        9,
        1,
        L"../res/floor.png",
        L"../MyLibrary/Plane.hlsl"
    };

    mMapObj = make_shared<Map>();
    mMapObj->Set(m_pDevice, m_pImmediateContext);
    mMapObj->CreateMap(mapDesc);
#pragma endregion

#pragma region 카메라 뷰행렬 생성, 프로젝션(투영)
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
    float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

    XMFLOAT3 eyeVec = { 0.0f, 10.0f, -5.0f };
    XMFLOAT3 lookAtVec = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 upVec = { 0.0f, 1.0f, 0.0f };

    mMainCamera->SetView(eyeVec, lookAtVec, upVec);
    mMainCamera->SetProjection(60.0f, aspect, 0.1f, 1000.0f);
#pragma endregion

    return true;
}
bool  sample::Frame()
{
    return true;
}
bool  sample::Render()
{
    m_pImmediateContext->OMSetBlendState(mAlphaBlend, 0, -1);
    XMMATRIX viewMatrix = mMainCamera->GetViewMatrix();
    XMMATRIX projMatrix = mMainCamera->GetProjMatrix();
    mMapObj->SetMatrix(nullptr, &viewMatrix, &projMatrix);
    mMapObj->Render();

    return true;
}
bool  sample::Release()
{
    mAlphaBlend->Release();
    mMapObj->Release();
    return true;
}

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) // 메인
{
    sample mySample;
    mySample.SetRegisterClassWindow(hInstance);
    mySample.SetWindow(L"아무거나", 1280, 720);
    ::ShowCursor(TRUE);
    mySample.Run();

    return 0;
}