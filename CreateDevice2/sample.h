#pragma once
#include "Core.h"
#include "PlaneObject.h"
#include "Player.h"

class sample : public Core
{
private:
    Object* m_pMapObj = nullptr;
    Object* mPlayer = nullptr;
    TextureManager m_texMgr;
    ShaderManager m_shaderMgr;
    std::vector<Object*> m_NpcList;
    ID3D11BlendState* m_AlphaBlend = nullptr;
    Matrix m_matView;
    Matrix m_matOrthoProjection;
    Vector3 m_vCameraPos;

    sample() {};
    virtual ~sample() {};
public:
    static sample& GetInstance()
    {
        static sample instance;
        return instance;
    }

    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};