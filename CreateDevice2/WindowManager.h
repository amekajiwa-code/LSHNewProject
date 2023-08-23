#pragma once
#include "Window.h"

class WindowManager
{
public:
	static WindowManager& GetInstance()
	{
		static WindowManager instance;
		return instance;
	}

	Window mWindow;

	bool InitializeWindow(HINSTANCE hInstance, const WCHAR* szTitle, DWORD dwWindowWidth, DWORD dwWindowHeight)
	{
		return mWindow.SetRegisterClassWindow(hInstance) &&
			mWindow.SetWindow(szTitle, dwWindowWidth, dwWindowHeight);
	}
private:
	WindowManager() {}
	virtual ~WindowManager() {}
};

