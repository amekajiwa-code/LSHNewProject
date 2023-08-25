#pragma once
#include "Core.h"
#include "PlaneObject.h"
#include "Player.h"
#include "NPC.h"

class sample : public Core
{
private:
    Object* mMapObj = nullptr;
    Object* mPlayer = nullptr;
    //TextureManager mTexMgr;
    //ShaderManager mShaderMgr;
    vector<Object*> mNpcList;
    ID3D11BlendState* mAlphaBlend = nullptr;

    Object* mSpriteObj = nullptr;
    vector<const Texture*>  mTexList;
    int mTexIndex = 0;
    
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();

    sample() {};
    virtual ~sample() {};
};