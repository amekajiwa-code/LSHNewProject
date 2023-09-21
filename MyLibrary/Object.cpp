#include "Object.h"

void Object::SetPos(XMFLOAT3 p)
{
    m_vPos = p;
}

void  Object::SetScale(XMFLOAT3 s)
{
    m_vScale = s;
}

void  Object::SetMatrix(XMMATRIX* matWorld, XMMATRIX* matView, XMMATRIX* matProj)
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
    m_cbData.matWorld = XMMatrixTranspose(m_matWorld);
    m_cbData.matView = XMMatrixTranspose(m_matView);
    m_cbData.matProj = XMMatrixTranspose(m_matProj);
    m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_cbData, 0, 0);
}

void Object::UpdateMatrix()
{
    XMMATRIX matScale, matRotation, matTranslate;
    matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z); // 스케일
    matRotation = XMMatrixRotationZ(m_vRotation.z); // z축회전
    matTranslate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z); //이동
    m_matWorld = matScale * matRotation * matTranslate;
}

bool  Object::Create(wstring texFilename, wstring shaderFilename)
{
    CreateConstantBuffer();
    CreateVertexData();
    CreateIndexData();
    CreateVertexBuffer();
    CreateIndexBuffer();
    m_pShader = ShaderManager::GetInstance().Load(shaderFilename); // InputLayOut 하기전에 꼭!! 쉐이더 로드하기
    CreateInputLayout();
    m_pTex = TextureManager::GetInstance().Load(texFilename);
    UpdateMatrix();
    return true;
}
void  Object::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext)
{
    m_pDevice = pDevice;
    m_pImmediateContext = pImmediateContext;
}
bool  Object::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(PNCT_Vertex) * m_VertexList.size();
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA InitialData;
    ZeroMemory(&InitialData, sizeof(InitialData));
    InitialData.pSysMem = &m_VertexList.at(0);

    HRESULT hr = m_pDevice->CreateBuffer(
        &Desc,
        &InitialData,
        &m_pVertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

bool  Object::CreateIndexBuffer()
{
    if (mIndexList.size() == 0) return true;
    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(DWORD) * mIndexList.size();
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA InitialData;
    ZeroMemory(&InitialData, sizeof(InitialData));
    InitialData.pSysMem = &mIndexList.at(0);

    HRESULT hr = m_pDevice->CreateBuffer(
        &Desc,
        &InitialData,
        &m_pIndexBuffer);
    if (FAILED(hr))
    {
        return false;
    }
    return true;
}

bool  Object::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(MATRIX_Data);
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
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

    UINT stride = sizeof(PNCT_Vertex);
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
    if (m_pIndexBuffer == nullptr)
    {
        m_pImmediateContext->Draw(m_VertexList.size(), 0);
    }
    else
    {
        m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        m_pImmediateContext->DrawIndexed(mIndexList.size(), 0, 0);
    }
    return true;
}

bool  Object::Release()
{
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pIndexBuffer) m_pIndexBuffer->Release();
    if (m_pVertexLayout) m_pVertexLayout->Release();
    if (m_pConstantBuffer)m_pConstantBuffer->Release();
    return true;
}
Object::Object()
{
    m_vPos = XMFLOAT3(0, 0, 0);
    m_vScale = XMFLOAT3(1, 1, 1);
    m_vRotation = XMFLOAT3(0, 0, 0);
}