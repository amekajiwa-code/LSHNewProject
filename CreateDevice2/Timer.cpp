#include "Timer.h"

bool Timer::Init() {
	mFramePerSecond = 0.0f;
	mTimer = 0.0f;
	mSecondPerFrame = 0.0f;
	mBeforeTime = timeGetTime();
	mFPS = 0;
	return true;
}

int   Timer::GetFPS()
{
	static int FPS = 0;
	if (mFramePerSecond >= 1.0f)
	{
		mFPS = FPS;
		FPS = 0;
		mFramePerSecond -= 1.0f;
	}
	FPS++;
	return mFPS;
}

bool Timer::Frame() {
	DWORD currentTime = timeGetTime();
	DWORD elapseTime = currentTime - mBeforeTime;
	mSecondPerFrame = elapseTime / 1000.0f;
	mTimer += mSecondPerFrame;
	mBeforeTime = currentTime;

	mFramePerSecond += mSecondPerFrame;
	return true;
}

bool Timer::Render() {
	return true;
}

bool Timer::Release() {
	return true;
}
