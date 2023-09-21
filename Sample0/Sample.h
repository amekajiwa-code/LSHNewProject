#pragma once
#include "Core.h"
#include "Map.h"
#include "DebugCamera.h"
#include "FBX_Import.h"
#pragma comment(lib, "MyLibrary.lib")

class sample : public Core
{
private:
    shared_ptr<Object> mBoxObj;
    shared_ptr<FBX_Import> mFbxImport;
    const Texture* mTex;

    ID3D11BlendState* mAlphaBlend = nullptr;
    shared_ptr<Map> mMapObj;
    shared_ptr<DebugCamera> mDebugCamera = make_shared<DebugCamera>();
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();

    sample() {};
    virtual ~sample() {};
};

