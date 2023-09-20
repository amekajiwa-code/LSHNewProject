#pragma once
#include "Core.h"
#include "Map.h"
#include "HeightMap.h"
#include "DebugCamera.h"
#pragma comment(lib, "MyLibrary.lib")

class sample : public Core
{
private:
    ID3D11BlendState* mAlphaBlend = nullptr;
    shared_ptr<Map> mMapObj;
    shared_ptr<DebugCamera> mDebugCamera = make_shared<DebugCamera>();
    HeightMap mHeightMap;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();

    sample() {};
    virtual ~sample() {};
};

