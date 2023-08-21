#include "PlaneObject.h"

bool PlaneObject::CreateVertexBuffer()
{
    //ÁÂÇ¥°è ±âÁØÀ¸·Î ¿ÞÂÊÀ§¿¡ »ï°¢Çü Çü¼º
    verticles.resize(6);
    // u, v
    verticles[0].t.mX = 0.0f; verticles[0].t.mY = 0.0f;
    verticles[1].t.mX = 1.0f; verticles[1].t.mY = 0.0f;
    verticles[2].t.mX = 0.0f; verticles[2].t.mY = 1.0f;
    verticles[3].t.mX = 0.0f; verticles[3].t.mY = 1.0f;
    verticles[4].t.mX = 1.0f; verticles[4].t.mY = 0.0f;
    verticles[5].t.mX = 1.0f; verticles[5].t.mY = 1.0f;
    // x, y, z
    verticles[0].p.mX = -1.0f; verticles[0].p.mY = 1.0f; verticles[0].p.mZ = 1.0f;
    verticles[1].p.mX = 1.0f; verticles[1].p.mY = 1.0f; verticles[1].p.mZ = 1.0f;
    verticles[2].p.mX = -1.0f; verticles[2].p.mY = -1.0f; verticles[2].p.mZ = 1.0f;
    verticles[3].p.mX = -1.0f; verticles[3].p.mY = -1.0f; verticles[3].p.mZ = 1.0f;
    verticles[4].p.mX = 1.0f; verticles[4].p.mY = 1.0f; verticles[4].p.mZ = 1.0f;
    verticles[5].p.mX = 1.0f; verticles[5].p.mY = -1.0f; verticles[5].p.mZ = 1.0f;

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(P3VERTEX) * verticles.size();
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = &verticles.at(0);

    HRESULT hResult = mDevice->CreateBuffer(&bufferDesc, &InitData, &mVertexBuffer);
    if (FAILED(hResult))
    {
        return false;
    }

    return true;
}

bool PlaneObject::Frame()
{
    Matrix matScale, matRotation, matTranslate; // SRT
    matScale.Scale(mScale);
    matRotation.ZRotate(mRotation.mZ);
    matTranslate.Translation(mPos);
    mMatWorld = matScale * matRotation * matTranslate;
    return true;
}