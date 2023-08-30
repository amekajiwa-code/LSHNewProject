#include "Timer.h"
#include "Writer.h"

float g_SecondPerFrame;
float g_GameTimer;

bool	Timer::Init()
{
	mFramePerSecond = 0.0f;
	mGameTimer = 0.0f;
	mSecondPerFrame = 0.0f;
	mDwBeforeTime = timeGetTime();
	return true;
}
bool	Timer::Frame()
{
	DWORD dwCurrentTime = timeGetTime();
	DWORD dwElapseTime = dwCurrentTime - mDwBeforeTime;
	g_SecondPerFrame = mSecondPerFrame = dwElapseTime / 1000.0f;
	g_GameTimer = mGameTimer += mSecondPerFrame;
	mDwBeforeTime = dwCurrentTime;

	mFramePerSecond += mSecondPerFrame;

	Writer::GetInstance().mTextList.clear();
	return true;
}
int   Timer::GetFPS()
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
bool	Timer::Render()
{
#ifdef _DEBUG
	std::cout << "[FPS]" << GetFPS() << " [GT]" << mGameTimer << " [SPF]" << mSecondPerFrame << std::endl;
	wstring fontFPS = L"FPS : " + to_wstring(GetFPS()) + L" SPF : " + to_wstring(mFramePerSecond) + L" Timer : " + to_wstring(mGameTimer);
	Writer::GetInstance().AddText(fontFPS, 10, 0, { 1.0f, 1.0f, 1.0f, 1.0f });
#endif
	return true;
}
bool	Timer::Release()
{
	return true;
}