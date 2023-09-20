#pragma once
#include "Std.h"
#include "TextureManager.h"
#include "ShaderManager.h"

struct PNCT_Vertex
{
    XMFLOAT3 p;
    XMFLOAT3 n;
    XMFLOAT4 c;
    XMFLOAT2 t;
};
struct MATRIX_Data
{
    XMMATRIX matWorld;
    XMMATRIX matView;
    XMMATRIX matProj;
};

class Object
{
public:
    XMMATRIX m_matWorld;
    XMMATRIX m_matView;
    XMMATRIX m_matProj;
    MATRIX_Data m_cbData;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
    XMFLOAT3 m_vPos;
    XMFLOAT3 m_vScale;
    XMFLOAT3 m_vRotation;
    void SetPos(XMFLOAT3 p);
    void SetScale(XMFLOAT3 s);
public:
    ID3D11Buffer* m_pVertexBuffer = nullptr;
    ID3D11Buffer* m_pIndexBuffer = nullptr;
    ID3D11Buffer* m_pConstantBuffer = nullptr;
    ID3D11InputLayout* m_pVertexLayout = nullptr;
    const Shader* m_pShader = nullptr;
    const Texture* m_pTex = nullptr;
    vector<PNCT_Vertex> m_VertexList;
    vector<DWORD> mIndexList;
public:
    void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);

    virtual bool  CreateVertexBuffer();
    virtual bool  CreateIndexBuffer();
    virtual bool  CreateConstantBuffer();
    virtual bool  CreateInputLayout();

    virtual bool  CreateVertexData()
    {
        return true;
    }
    virtual bool  CreateIndexData()
    {
        return true;
    }
public:
    virtual bool  Create(wstring texFilename, wstring shaderFilename);
    virtual bool  Init();
    virtual bool  Frame();
    virtual bool  PreRender();
    virtual bool  Render();
    virtual bool  PostRender();
    virtual bool  Release();
    virtual void  SetMatrix(XMMATRIX* matWorld, XMMATRIX* matView, XMMATRIX* matProj);
    virtual void  UpdateMatrix();
    virtual void  Move() {};
public:
    Object();
    virtual ~Object() {}
};
