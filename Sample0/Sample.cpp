#include "Sample.h"

bool  sample::Init()
{
#pragma region ���ĺ���
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

#pragma region ��Ʈ
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

#pragma region �ʿ�����Ʈ ����
    MapDesc mapDesc = {
        9,
        9,
        1,
        0,
        L"../res/bg.jpg",
        L"../MyLibrary/Plane.hlsl"
    };
    mMapObj = make_shared<Map>();
    mMapObj->Set(m_pDevice, m_pImmediateContext);
    mMapObj->CreateMap(mapDesc);
#pragma endregion

#pragma region ī�޶� ����� ����, ��������(����)
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
    float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

    XMFLOAT3 eyeVec = { 0.0f, 10.0f, -10.0f };
    XMFLOAT3 lookAtVec = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 upVec = { 0.0f, 1.0f, 0.0f };

    mDebugCamera->Init();
    mDebugCamera->SetView(eyeVec, lookAtVec, upVec);
    mDebugCamera->SetProjection(45.0f, aspect, 1.0f, 1000.0f);

    mMainCamera = mDebugCamera; // ������ �Ѱ���
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
    XMMATRIX worldMatrix = XMMatrixIdentity();
    XMMATRIX viewMatrix = mMainCamera->GetViewMatrix();
    XMMATRIX projMatrix = mMainCamera->GetProjMatrix();
    mMapObj->SetMatrix(&worldMatrix, &viewMatrix, &projMatrix);
    mMapObj->Render();

    XMMATRIX worldMatrix2 = XMMatrixIdentity();
    XMMatrixInverse(nullptr, worldMatrix2);
    XMMatrixTranspose(worldMatrix2);
    worldMatrix2.r[3].m128_f32[1] = -3.0f; // _42 y�� �̵�
    viewMatrix = mMainCamera->GetViewMatrix();
    projMatrix = mMainCamera->GetProjMatrix();
    mMapObj->SetMatrix(&worldMatrix2, &viewMatrix, &projMatrix);
    mMapObj->Render();

    return true;
}
bool  sample::Release()
{
    return true;
}

//int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) // ����
//{
//    sample mySample;
//    mySample.SetRegisterClassWindow(hInstance);
//    mySample.SetWindow(L"�ƹ��ų�", 1280, 720);
//    ::ShowCursor(TRUE);
//    mySample.Run();
//
//    return 0;
//}