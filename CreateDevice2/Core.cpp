#include "Core.h"

bool  Core::Init() { return true; }
bool  Core::Frame() { return true; }
bool  Core::Render() { return true; }
bool  Core::Release() { return true; }

bool  Core::EngineInit()
{
    Device::Init();
    mGameTimer.Init();
    Input::GetInstance().Init();
    Writer::GetInstance().Init();
    SoundManager::GetInstance().Init();
    mMainCamera.Init();
    Init();

    return true;
}

bool  Core::EngineFrame()
{
    mGameTimer.Frame();
    Input::GetInstance().Frame();
    Writer::GetInstance().Frame();
    SoundManager::GetInstance().Frame();
    mMainCamera.Frame();
    Device::Frame();
    Frame();

    return true;
}

bool  Core::EngineRender()
{
    Device::PreRender();
    Render();
    mGameTimer.Render();
    Input::GetInstance().Render();
    Writer::GetInstance().Render();
    mMainCamera.Render();
    Device::PostRender();

    return true;
}

bool  Core::EngineRelease()
{
    Release();
    mGameTimer.Release();
    Input::GetInstance().Release();
    Writer::GetInstance().Release();
    SoundManager::GetInstance().Release();
    mMainCamera.Release();
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