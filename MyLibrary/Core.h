#pragma once
#include "Device.h"
#include "Timer.h"
#include "Input.h"
#include "Camera.h"
#include "Writer.h"
#include "SoundManager.h"

class Core : public Device
{
public:
    shared_ptr<Camera> mMainCamera = make_shared<Camera>();
    Sound* mSound = nullptr;

    ComPtr<ID3D11SamplerState> m_pSamplerState;
    ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
    ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateDisable;
    ComPtr<ID3D11RasterizerState> m_pRSWireFrame;
    ComPtr<ID3D11RasterizerState> m_pRSSolid;

    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
    bool  Run();
    void CreateDepthStencilState();
    virtual void ResizeDevice(UINT width, UINT height) override;
    virtual bool DeleteDxResource();
    virtual bool CreateDxResource();
    virtual void CreateRasterizerState();
private:
    virtual bool  EngineInit();
    virtual bool  EngineFrame();
    virtual bool  EngineRender();
    virtual bool  EngineRelease();
};