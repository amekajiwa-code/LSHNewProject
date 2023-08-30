#pragma once
#include <chrono>
#include <thread>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "winmm.lib") //timeGetTime()
class Timer
{
public:
	float mGameTimer;
	float mSecondPerFrame;
	std::chrono::high_resolution_clock::time_point mBeforeTime;
	float mFramePerSecond;
	int mFPS;
public:
	int		GetFPS();
public:
	bool	Init();		// 초기화 작업
	bool	Frame();	// 실시간 계산
	bool	Render();	// 실시간 랜더링, 드로우
	bool	Release();	// 객체의 소멸 작업
};