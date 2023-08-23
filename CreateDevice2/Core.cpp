#include "Core.h"
bool  Core::Init() { return true; }
bool  Core::Frame() { return true; }
bool  Core::Render() { return true; }
bool  Core::Release() { return true; }
bool  Core::EngineInit()
{
    Device::Init();
    m_GameTimer.Init();
    Init();
    return true;
}
bool  Core::EngineFrame()
{
    m_GameTimer.Frame();
    Device::Frame();
    Frame();
    return true;
}
bool  Core::EngineRender()
{
    Device::PreRender();
    Render();
    m_GameTimer.Render();
    Device::PostRender();
    return true;
}
bool  Core::EngineRelease()
{
    Release();
    m_GameTimer.Release();
    Device::Release();
    return true;
}
bool Core::Run()
{
    EngineInit();
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (!EngineFrame() || !EngineRender())
            {
                break;
            }
        }
    }
    EngineRelease();
    return true;
}