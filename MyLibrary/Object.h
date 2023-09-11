#pragma once
#include "Std.h"
#include "TextureManager.h"
#include "ShaderManager.h"

struct PT_Vertex
{
    Vector3 p;
    Vector2 t;
};
struct CB_Data
{
    Matrix matWorld;
    Matrix matView;
    Matrix matProj;
};

class Object
{
public:
    bool m_bDead = false;
    bool isInvincible = false;
    Matrix m_matWorld;
    Matrix m_matView;
    Matrix m_matProj;
    CB_Data m_cbData;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
    Vector3 m_vPos;
    Vector3 m_vScale;
    Vector3 m_vRotation;
    Rect mRect;
    void SetPos(Vector3 p);
    void SetScale(Vector3 s);
    virtual void SetRect(Vector2& center, float width, float height);
public:
    ID3D11Buffer* m_pVertexBuffer = nullptr;
    ID3D11Buffer* m_pConstantBuffer = nullptr;
    ID3D11InputLayout* m_pVertexLayout = nullptr;

    const Shader* m_pShader = nullptr;
    const Texture* m_pTex = nullptr;
    std::vector< PT_Vertex> m_VertexList;
public:
    string mTag = "";
    void SetTag(string tag);
    string GetTag();
public:
    void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
    virtual bool  CreateVertexBuffer();
    virtual bool  CreateConstantBuffer();
    virtual bool  CreateInputLayout();
public:
    virtual bool  Create(TextureManager& texMgr, std::wstring shaderFilename,
        ShaderManager& shaderMgr, std::wstring texFilename);
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  PreRender();
    virtual bool  Render();
    virtual bool  PostRender();
    virtual bool  Release();
    virtual void  SetMatrix(Matrix* matWorld, Matrix* matView, Matrix* matProj);
    virtual void  Move() {};
public:
    bool GetInvincible() { return isInvincible; }
    void Setinvincible(bool invincible) { isInvincible = invincible; }
public:
    Object();
    virtual ~Object() {}
};
