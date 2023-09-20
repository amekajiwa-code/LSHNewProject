#pragma once
#include "Core.h"
#include "Map.h"
#include "HeightMap.h"
#include "DebugCamera.h"
#pragma comment(lib, "MyLibrary.lib")

class sample2 : public Core
{
private:
    ID3D11BlendState* mAlphaBlend = nullptr;
    shared_ptr<HeightMap> mHeightMap;
    shared_ptr<DebugCamera> mDebugCamera = make_shared<DebugCamera>();
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();

    sample2() {};
    virtual ~sample2() {};
};

