#pragma once
#include "Window.h"
class Device : public Window
{
public:
    DXGI_SWAP_CHAIN_DESC mSwapChainDesc;
    IDXGISwapChain* m_pSwapChain = nullptr;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
    ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
    D3D11_VIEWPORT m_ViewPort;

    bool  Init();
    bool  Frame();
    bool  PreRender();
    bool  PostRender();
    bool  Render();
    bool  Release();

    bool SetRenderTargetView();
    bool SetDepthStencilView();
    bool SetViewPort();
};