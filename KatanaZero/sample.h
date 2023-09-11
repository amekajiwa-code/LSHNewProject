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

    vector<Object*> mNpcEffectObjList; // npc ����Ʈ ������Ʈ �迭

    Vector3 curMouse = { 0, 0, 0 };

    int mTexIndex = 0;
    int mEffectIndex = 0;

    int mNpcEffectIndex = 0; // npc ����Ʈ �ε���

    vector<const Texture*>  mTexList;

    vector<Npc*> mNpcList; // npc ����Ʈ
    vector<Gunman*> mGunmanList;

    vector<Object*> mFloorList;

    vector<const Texture*>  mEffectList;

    vector<const Texture*>  mNpcEffectList; // npc ��������Ʈ �迭

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