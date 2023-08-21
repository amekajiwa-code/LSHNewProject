#pragma once
#include "Device.h"
#include "Timer.h"

class Core : public Device
{
protected:
    Timer mGameTimer;
public:
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

