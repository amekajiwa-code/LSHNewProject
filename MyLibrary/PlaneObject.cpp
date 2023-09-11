#include "PlaneObject.h"
bool  PlaneObject::CreateVertexBuffer()
{
    m_VertexList.resize(6);
    m_VertexList[0].t.mX = 0.0f; m_VertexList[0].t.mY = 0.0f;
    m_VertexList[1].t.mX = 1.0f; m_VertexList[1].t.mY = 0.0f;
    m_VertexList[2].t.mX = 0.0f; m_VertexList[2].t.mY = 1.0f;
    m_VertexList[3].t.mX = 0.0f; m_VertexList[3].t.mY = 1.0f;
    m_VertexList[4].t.mX = 1.0f; m_VertexList[4].t.mY = 0.0f;
    m_VertexList[5].t.mX = 1.0f; m_VertexList[5].t.mY = 1.0f;

    m_VertexList[0].p.mX = -1.0f; m_VertexList[0].p.mY = 1.0f;  m_VertexList[0].p.mZ = 0.0f;
    m_VertexList[1].p.mX = 1.0f; m_VertexList[1].p.mY = 1.0f;  m_VertexList[1].p.mZ = 0.0f;
    m_VertexList[2].p.mX = -1.0f; m_VertexList[2].p.mY = -1.0f;  m_VertexList[2].p.mZ = 0.0f;
    m_VertexList[3] = m_VertexList[2];
    m_VertexList[4] = m_VertexList[1];
    m_VertexList[5].p.mX = 1.0f; m_VertexList[5].p.mY = -1.0f;  m_VertexList[5].p.mZ = 0.0f;

    D3D11_BUFFER_DESC Desc;
    ZeroMemory(&Desc, sizeof(Desc));
    Desc.ByteWidth = sizeof(PT_Vertex) * m_VertexList.size();
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

bool PlaneObject::Frame()
{
    Matrix matScale, matRotation, matTranslate;
    matScale.Scale(m_vScale);
    matRotation.ZRotate(m_vRotation.mZ);
    matTranslate.Translation(m_vPos);
    m_matWorld = matScale * matRotation * matTranslate;
    return true;
}