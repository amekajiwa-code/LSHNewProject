#include "PlaneObject.h"
bool  PlaneObject::CreateVertexBuffer()
{
    m_VertexList.resize(6);
    m_VertexList[0].t.x = 0.0f; m_VertexList[0].t.y = 0.0f;
    m_VertexList[1].t.x = 1.0f; m_VertexList[1].t.y = 0.0f;
    m_VertexList[2].t.x = 0.0f; m_VertexList[2].t.y = 1.0f;
    m_VertexList[3].t.x = 0.0f; m_VertexList[3].t.y = 1.0f;
    m_VertexList[4].t.x = 1.0f; m_VertexList[4].t.y = 0.0f;
    m_VertexList[5].t.x = 1.0f; m_VertexList[5].t.y = 1.0f;

    m_VertexList[0].p.x = -1.0f; m_VertexList[0].p.y = 1.0f;  m_VertexList[0].p.z = 0.0f;
    m_VertexList[1].p.x = 1.0f; m_VertexList[1].p.y = 1.0f;  m_VertexList[1].p.z = 0.0f;
    m_VertexList[2].p.x = -1.0f; m_VertexList[2].p.y = -1.0f;  m_VertexList[2].p.z = 0.0f;
    m_VertexList[3] = m_VertexList[2];
    m_VertexList[4] = m_VertexList[1];
    m_VertexList[5].p.x = 1.0f; m_VertexList[5].p.y = -1.0f;  m_VertexList[5].p.z = 0.0f;

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

bool PlaneObject::Frame()
{
    XMMATRIX matScale, matRotation, matTranslate;
    matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z); // 스케일
    matRotation = XMMatrixRotationZ(m_vRotation.z); // z축회전
    matTranslate = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z); //이동
    m_matWorld = matScale * matRotation * matTranslate;
    return true;
}