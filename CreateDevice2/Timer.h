#pragma once
#include <windows.h>
#pragma comment (lib, "Winmm.lib")

class Timer
{
public:
	float mSecondPerFrame;
	float mTimer;
	int mFPS;
	float mFramePerSecond;
	DWORD mBeforeTime;
	int GetFPS();
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};