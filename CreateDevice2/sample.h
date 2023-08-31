#pragma once
#include "Core.h"
#include "PlaneObject.h"
#include "Player.h"
#include "NPC.h"

class sample : public Core
{
private:
    ID3D11BlendState* mAlphaBlend = nullptr;

    Object* mMapObj = nullptr;
    Player* mPlayer = nullptr;
    Object* mFloorObj = nullptr;
    Object* mCursorObj = nullptr;
    Object* mEffectObj = nullptr;

    int mTexIndex = 0;
    int mEffectIndex = 0;

    vector<const Texture*>  mTexList;
    vector<Object*> mNpcList;
    vector<Object*> mFloorList;
    vector<const Texture*>  mEffectList;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();

    sample() {};
    virtual ~sample() {};
};