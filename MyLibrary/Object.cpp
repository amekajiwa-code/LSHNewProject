#include "Object.h"

void Object::SetTag(string tag)
{
    mTag = tag;
}

string Object::GetTag()
{
    return mTag;
}

void Object::SetRect(Vector2& center, float width, float height)
{
    mRect.Set(center, width, height);
}

void Object::SetPos(Vector3 p)
{
    m_vPos = p;
}

void  Object::SetScale(Vector3 s)
{
    m_vScale = s;
}

void  Object::SetMatrix(Matrix* matWorld, Matrix* matView, Matrix* matProj)
{
    if (matWorld != nullptr)
    {
        m_matWorld = *matWorld;
    }
    if (matView != nullptr)
    {
        m_matView = *matView;
    }
    if (matProj != nullptr)
    {
        m_matProj = *matProj;
    }
    m_cbData.matWorld = m_matWorld.Transpose();
    m_cbData.matView = m_matView.Transpose();
    m_cbData.matProj = m_matProj.Transpose();
    m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_cbData, 0, 0);
}
bool  Object::Create(TextureManager& texMgr, std::wstring texFilename,
    ShaderManager& shaderMgr, std::wstring shaderFilename)
{
    CreateConstantBuffer();
    CreateVertexBuffer();
    m_pShader = shaderMgr.Load(shaderFilename);
    CreateInputLayout();
    m_pTex = texMgr.Load(texFilename);
    return true;
}
void  Object::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext)
{
    m_pDevice = pDevice;
    m_pImmediateContext = pImmediateContext;
}
bool  Object::CreateVertexBuffer()
{
    return true;
}
bool  Object::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(CB_Data);
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hr = m_pDevice->CreateBuffer(
        &Desc,
        nullptr,
        &m_pConstantBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}
bool  Object::CreateInputLayout()
{
    const D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POS",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT iNumCount = sizeof(layout) / sizeof(layout[0]);

    if (m_pShader)
    {
        HRESULT hr = m_pDevice->CreateInputLayout(
            layout,
            iNumCount,
            m_pShader->GetBufferPointer(),
            m_pShader->GetBufferSize(),
            &m_pVertexLayout);
        if (FAILED(hr))
        {
            return false;
        }
    }
    return true;
}
bool  Object::Init()
{

    return true;
}
bool  Object::Frame()
{
    return true;
}

bool Object::PreRender()
{
    m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    if (m_pTex)
    {
        m_pTex->Apply(m_pImmediateContext, 0);
    }
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
    if (m_pShader)
    {
        m_pShader->Apply(m_pImmediateContext, 0);
    }

    UINT stride = sizeof(PT_Vertex);
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
    m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool  Object::Render()
{
    PreRender();
    PostRender();
    return true;
}

bool Object::PostRender()
{
    m_pImmediateContext->Draw(m_VertexList.size(), 0);
    return true;
}

bool  Object::Release()
{
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pVertexLayout) m_pVertexLayout->Release();
    if (m_pConstantBuffer)m_pConstantBuffer->Release();
    return true;
}
Object::Object()
{
    m_vPos = Vector3(0, 0, 0);
    m_vScale = Vector3(1, 1, 1);
    m_vRotation = Vector3(0, 0, 0);
}