#include "Timer.h"

bool	Timer::Init()
{
	m_fFramePerSecond = 0.0f;
	m_fGameTimer = 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_dwBeforeTime = timeGetTime();
	return true;
}
bool	Timer::Frame()
{
	DWORD dwCurrentTime = timeGetTime();
	DWORD dwElapseTime = dwCurrentTime - m_dwBeforeTime;
	m_fSecondPerFrame = dwElapseTime / 1000.0f;
	m_fGameTimer += m_fSecondPerFrame;
	m_dwBeforeTime = dwCurrentTime;

	m_fFramePerSecond += m_fSecondPerFrame;
	return true;
}
int   Timer::GetFPS()
{
	static int iFPS = 0;
	if (m_fFramePerSecond >= 1.0f)
	{
		m_iFPS = iFPS;
		iFPS = 0;
		m_fFramePerSecond -= 1.0f;
	}
	iFPS++;
	return m_iFPS;
}
bool	Timer::Render()
{
	std::cout << "[FPS]" << GetFPS() << " [GT]" << m_fGameTimer << " [SPF]" << m_fSecondPerFrame << std::endl;
	return true;
}
bool	Timer::Release()
{
	return true;
}