#pragma once
#include "Window.h"
class Device : public Window
{
public:
    IDXGISwapChain* m_pSwapChain = nullptr;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
    ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
    D3D11_VIEWPORT m_ViewPort;

    bool  Init();
    bool  Frame();
    bool  PreRender();
    bool  PostRender();
    bool  Render();
    bool  Release();
};