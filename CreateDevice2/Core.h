#pragma once
#include "Device.h"
#include "Timer.h"
#include "Input.h"
#include "Camera.h"
#include "Writer.h"

class Core : public Device
{
public:
    Timer mGameTimer;
    Camera mMainCamera;

    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  Render();
    virtual bool  Release();
    bool  Run();
private:
    virtual bool  EngineInit();
    virtual bool  EngineFrame();
    virtual bool  EngineRender();
    virtual bool  EngineRelease();
};