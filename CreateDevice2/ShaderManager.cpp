#include "ShaderManager.h"

bool Shader::LoadVertexShader(ID3D11Device* device, wstring fileName)
{
    ID3DBlob* ErrorCode;
    // 버텍스 쉐이더 컴파일
    HRESULT hResult = D3DCompileFromFile(
        fileName.c_str(),
        nullptr,
        nullptr,
        "VS",
        "vs_5_0",
        0,
        0,
        &mVertexShaderCode,
        &ErrorCode);
    if (FAILED(hResult))
    {
        return false;
    }
    hResult = device->CreateVertexShader(
        mVertexShaderCode->GetBufferPointer(),
        mVertexShaderCode->GetBufferSize(),
        nullptr,
        &mVS);

    if (ErrorCode) ErrorCode->Release();
    return true;
}

bool Shader::LoadPixelShader(ID3D11Device* device, wstring fileName)
{
    ID3DBlob* ShaderCode;
    ID3DBlob* ErrorCode;
    // 픽셀 쉐이더 컴파일
    HRESULT hResult = D3DCompileFromFile(
        fileName.c_str(),
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &ShaderCode,
        &ErrorCode);
    if (FAILED(hResult))
    {
        return false;
    }
    hResult = device->CreatePixelShader(
        ShaderCode->GetBufferPointer(),
        ShaderCode->GetBufferSize(),
        nullptr,
        &mPS);

    if (ShaderCode) ShaderCode->Release();
    if (ErrorCode) ErrorCode->Release();
    return true;
}

bool Shader::Load(ID3D11Device* device, wstring fileName)
{
	LoadVertexShader(device, fileName);
	LoadPixelShader(device, fileName);
	return true;
}

bool Shader::Release()
{
	if (mVertexShaderCode)
	{
		mVertexShaderCode->Release();
		mVertexShaderCode = nullptr;
	}
	if (mVS) {
		mVS->Release();
		mVS = nullptr;
	}
	if (mPS) {
		mPS->Release();
		mPS = nullptr;
	}
	return true;
}

void ShaderManager::Set(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
    mDevice = device;
    mImmediateContext = immediateContext;
}

const Shader* ShaderManager::Load(wstring filePath)
{
    size_t found = filePath.find_last_of(L"/");
    wstring path = filePath.substr(0, found + 1);
    wstring key = filePath.substr(found + 1);
    const Shader* data = GetPtr(key);

    if (data != nullptr) {
        return data;
    }
    Shader* newData = new Shader;
    if (newData->Load(mDevice, filePath)) {
        mShaList.insert(make_pair(key, newData));
        return newData;
    }
    delete newData;
    return nullptr;
}

const Shader* ShaderManager::GetPtr(wstring key)
{
    auto iter = mShaList.find(key);
    if (mShaList.end() == iter)
    {
        return nullptr;
    }
    return iter->second;
}

bool ShaderManager::Get(wstring key, Shader& ret)
{
    auto iter = mShaList.find(key);
    if (mShaList.end() == iter)
    {
        return false;
    }
    ret = *(iter->second);
    return true;
}

ShaderManager::ShaderManager() {}
ShaderManager::~ShaderManager()
{
    for (auto& data : mShaList)
    {
        data.second->Release();
        delete data.second;
    }
    mShaList.clear();
}