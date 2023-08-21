#include "Device.h"

bool Device::Init() {
    // 스왑체인 속성값 구조체: 더블버퍼링을 구현하는데 사용됨
    DXGI_SWAP_CHAIN_DESC SwapChainDesc; // 스왑체인 속성값 구조체: 더블버퍼링을 구현하는데 사용됨
    ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = mHWnd;
    SwapChainDesc.Windowed = true;
    //백 버퍼 세팅
    SwapChainDesc.BufferDesc.Width = mDwWindowWidth; // 버퍼 크기
    SwapChainDesc.BufferDesc.Height = mDwWindowHeight;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // 버퍼 주사율 분자
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // 버퍼 주사율 분모
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 색 포멧
    //백 버퍼 멀티샘플링 count
    SwapChainDesc.SampleDesc.Count = 1;

    D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;
    UINT Flags = 0;
    D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0; // 최소 버전 DX11

    //1) 디바이스 설정
    HRESULT hResult = D3D11CreateDeviceAndSwapChain(
        NULL,
        DriverType,
        NULL,
        Flags,
        &FeatureLevels,
        1,
        D3D11_SDK_VERSION,
        &SwapChainDesc,
        &mSwapChain, // 백버퍼 인터페이스
        &mDevice, // dx 인터페이스 (생성)
        NULL,
        &mImmediateContext // dx 인터페이스 (관리)
    );

    if (FAILED(hResult)) return false;

    //2) 백버퍼 가져옴
    ID3D11Texture2D* backBuffer; // 백버퍼 포인터
    hResult = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)/*인터페이스 유형*/, (LPVOID*)&backBuffer);//스왑체인의 백버퍼 가져옴

    if (SUCCEEDED(hResult)) {
        //3) 렌더타겟 지정
        hResult = mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);

        if (FAILED(hResult)) {
            backBuffer->Release();
            return false;
        }
        mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);
    }
    backBuffer->Release();

    //뷰포트 생성
    mViewPort.Width = static_cast<float>(mDwWindowWidth);
    mViewPort.Height = static_cast<float>(mDwWindowHeight);
    mViewPort.MinDepth = 0.0f;
    mViewPort.MaxDepth = 1.0f;
    mViewPort.TopLeftX = 0;
    mViewPort.TopLeftY = 0;
    mImmediateContext->RSSetViewports(1, &mViewPort); // 레스터라이저

    return true;
};

bool Device::Frame() {
    return true;
};

bool  Device::PreRender()
{
    float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    mImmediateContext->ClearRenderTargetView(mRenderTargetView, color);
    return true;
}
bool  Device::PostRender()
{
    HRESULT hr = mSwapChain->Present(0, 0);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

bool Device::Render() {
    PreRender();
    PostRender();
    return true;
};

bool Device::Release() {
    if (mSwapChain) mSwapChain->Release();
    if (mDevice) mDevice->Release();
    if (mImmediateContext) mImmediateContext->Release();
    if (mRenderTargetView) mRenderTargetView->Release();
    return true;
};