#pragma once
#include "Core.h"
#include "PlaneObject.h"
#include "Player.h"
#include "NPC.h"
#include "Gunman.h"
#include "OBB.h"
#include "GameManager.h"

#pragma comment(lib, "MyLibrary.lib")

class sample : public Core
{
private:
    ID3D11BlendState* mAlphaBlend = nullptr;

    Object* mMapObj = nullptr;
    Player* mPlayer = nullptr;
    Object* mFloorObj = nullptr;
    Object* mCursorObj = nullptr;
    Object* mEffectObj = nullptr;

    vector<Object*> mNpcEffectObjList; // npc 이펙트 오브젝트 배열

    Vector3 curMouse = { 0, 0, 0 };

    int mTexIndex = 0;
    int mEffectIndex = 0;

    int mNpcEffectIndex = 0; // npc 이펙트 인덱스

    vector<const Texture*>  mTexList;

    vector<Npc*> mNpcList; // npc 리스트
    vector<Gunman*> mGunmanList;

    vector<Object*> mFloorList;

    vector<const Texture*>  mEffectList;

    vector<const Texture*>  mNpcEffectList; // npc 스프라이트 배열

    OBB obb;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();

    void CameraViberation();

    sample() {};
    virtual ~sample() {};
};