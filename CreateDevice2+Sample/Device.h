#pragma once
#include "Window.h"

class Device : public Window {
public:
	IDXGIFactory* mFactory = nullptr;
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mImmediateContext = nullptr;
	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	D3D11_VIEWPORT mViewPort;
public:
	bool  Init();
	bool  Frame();
	bool  PreRender();
	bool  PostRender();
	bool  Render();
	bool  Release();

	ID3D11Device* GetDevice() {
		return mDevice;
	}

	ID3D11DeviceContext* GetContext() {
		return mImmediateContext;
	}
};