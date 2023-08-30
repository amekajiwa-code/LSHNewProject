#include "Timer.h"
#include "Writer.h"

float g_SecondPerFrame;
float g_GameTimer;

bool Timer::Init()
{
	mFramePerSecond = 0.0f;
	mGameTimer = 0.0f;
	mSecondPerFrame = 0.0f;
	mBeforeTime = std::chrono::high_resolution_clock::now();
	return true;
}
bool Timer::Frame()
{
	chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapseTime = currentTime - mBeforeTime;
	g_SecondPerFrame = mSecondPerFrame = elapseTime.count();
	g_GameTimer = mGameTimer += mSecondPerFrame;
	mBeforeTime = currentTime;

	mFramePerSecond += mSecondPerFrame;

	Writer::GetInstance().mTextList.clear();
	return true;
}
int Timer::GetFPS()
{
	static int iFPS = 0;
	if (mFramePerSecond >= 1.0f)
	{
		mFPS = iFPS;
		iFPS = 0;
		mFramePerSecond -= 1.0f;
	}
	iFPS++;
	return mFPS;
}
bool Timer::Render()
{
#ifdef _DEBUG
	std::cout << "[FPS]" << GetFPS() << " [GT]" << mGameTimer << " [SPF]" << mSecondPerFrame << std::endl;
	wstring fontFPS = L"FPS : " + to_wstring(GetFPS()) + L" SPF : " + to_wstring(mFramePerSecond) + L" Timer : " + to_wstring(mGameTimer);
	Writer::GetInstance().AddText(fontFPS, 10, 0, { 1.0f, 1.0f, 1.0f, 1.0f });
#endif
	return true;
}
bool Timer::Release()
{
	return true;
}