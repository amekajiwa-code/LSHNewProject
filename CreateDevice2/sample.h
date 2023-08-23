#pragma once
#include "Core.h"
#include "PlaneObject.h"

class sample : public Core
{
    Object* m_pMapObj = nullptr;
    TextureManager             m_texMgr;
    ShaderManager              m_shaderMgr;
    std::vector<Object*>   m_NpcList;
    ID3D11BlendState* m_AlphaBlend = nullptr;
    Matrix                 m_matView;
    Matrix                 m_matOrthoProjection;
    Vector3                m_vCameraPos;
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();
};