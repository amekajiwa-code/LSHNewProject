#include "Object.h"

void Object::Set(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
    mDevice = device;
    mImmediateContext = immediateContext;
}

bool Object::Create(TextureManager& texMg, wstring texFileName, ShaderManager& shaMg, wstring shaFileName)
{
    CreateVertexBuffer();
    CreateConstantBuffer();
    mTexture = texMg.Load(texFileName);
    mShader = shaMg.Load(shaFileName);
    //LoadVertexShader();
    //LoadPixelShader();
    InputLayout();
    //LoadTextureFile(texFileName);
    
    return true;
}

void Object::SetPos(Vector3 pos)
{
    mPos = pos;
}

void Object::SetScale(Vector3 scale)
{
    mScale = scale;
}

void Object::SetMatrix(Matrix* matWorld, Matrix* matView, Matrix* matProjection)
{
    if (matWorld != nullptr)
    {
        mMatWorld = *matWorld;
        mMatData.matWorld = mMatWorld.Transpose();
    }
    
    if (matView != nullptr)
    {
        mMatView = *matView;
        mMatData.matView = mMatView.Transpose();
    }

    if (matProjection != nullptr)
    {
        mMatProjection = *matProjection;
        mMatData.matProjection = mMatProjection.Transpose();
    }
    
    mMatData.matWorld = mMatWorld.Transpose();
    mMatData.matView = mMatView.Transpose();
    mMatData.matProjection = mMatProjection.Transpose();
    mImmediateContext->UpdateSubresource(mConstantBuffer, 0, nullptr, &mMatData, 0, 0);
}

bool Object::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(MatrixData);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    HRESULT hResult = mDevice->CreateBuffer(&bufferDesc, nullptr, &mConstantBuffer);
    if (FAILED(hResult))
    {
        return false;
    }

    return true;
}

bool Object::CreateVertexBuffer()
{
    return true;

    //ÁÂÇ¥°è ±âÁØÀ¸·Î ¿ŞÂÊÀ§¿¡ »ï°¢Çü Çü¼º
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

bool Object::InputLayout()
{
    const D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numCount = sizeof(layout) / sizeof(layout[0]);

    if (mShader) {
        HRESULT hResult = mDevice->CreateInputLayout(
            layout,
            numCount,
            mShader->GetBufferPointer(),
            mShader->GetBufferSize(),
            &mVertexLayout);

        if (FAILED(hResult)) {
            return false;
        }
    }
  
    return true;
}

bool Object::Init()
{
    return true;
}

bool Object::Frame()
{
    return true;
}

bool Object::Render()
{
    ID3D11ShaderResourceView* texSRV = nullptr;

    if (mTexture)
    {
        mTexture->Apply(mImmediateContext, 0);
    }

    mImmediateContext->IASetInputLayout(mVertexLayout);

    if (mShader)
    {
        mShader->Apply(mImmediateContext, 0);
    }

    UINT stride = sizeof(P3VERTEX);
    UINT offset = 0;
    mImmediateContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    mImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mImmediateContext->Draw(verticles.size(), 0);
    return true;
}

bool Object::Release()
{
    if (mVertexBuffer) mVertexBuffer->Release();
    if (mVertexLayout) mVertexLayout->Release();

    return true;
}