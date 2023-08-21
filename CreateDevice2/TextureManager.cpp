#include "TextureManager.h"

bool Texture::Release()
{
    if (mTexSRV) {
        mTexSRV->Release();
        mTexSRV = nullptr;
        return true;
    }
    return false;
}

bool Texture::Load(ID3D11Device* device, wstring fileName)
{
    auto imageobj = std::make_unique<DirectX::ScratchImage>();
        DirectX::TexMetadata mdata;

        HRESULT hr = DirectX::GetMetadataFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, mdata);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS_NONE, &mdata, *imageobj);
            if (SUCCEEDED(hr))
            {
                hr = DirectX::CreateShaderResourceView(device, imageobj->GetImages(), imageobj->GetImageCount(), mdata, &mTexSRV);
                if (SUCCEEDED(hr))
                {
                    return true;
                }
            }
        }
        hr = DirectX::GetMetadataFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, mdata);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS_NONE, &mdata, *imageobj);
            if (SUCCEEDED(hr))
            {
                hr = DirectX::CreateShaderResourceView(device, imageobj->GetImages(), imageobj->GetImageCount(), mdata, &mTexSRV);
                if (SUCCEEDED(hr))
                {
                    return true;
                }
            }
        }
        hr = DirectX::GetMetadataFromTGAFile(fileName.c_str(), DirectX::TGA_FLAGS_NONE, mdata);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::LoadFromTGAFile(fileName.c_str(), DirectX::TGA_FLAGS_NONE, &mdata, *imageobj);
            if (SUCCEEDED(hr))
            {
                hr = DirectX::CreateShaderResourceView(device, imageobj->GetImages(), imageobj->GetImageCount(), mdata, &mTexSRV);
                if (SUCCEEDED(hr))
                {
                    return true;
                }
            }
        }
}

void TextureManager::Set(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{
    mDevice = device;
    mImmediateContext = immediateContext;
}

const Texture* TextureManager::Load(wstring filePath)
{
    size_t found = filePath.find_last_of(L"/");
    wstring path = filePath.substr(0, found + 1);
    wstring key = filePath.substr(found + 1);
    const Texture* data = GetPtr(key);

    if (data != nullptr) {
        return data;
    }
    Texture* newData = new Texture;
    if (newData->Load(mDevice, filePath)) {
        mTexList.insert(make_pair(key, newData));
        return newData;
    }
    delete newData;
    return nullptr;
}

const Texture* TextureManager::GetPtr(wstring key)
{
    auto iter = mTexList.find(key);
    if (mTexList.end() == iter) {
        return nullptr;
    }
	return iter->second;
}

bool TextureManager::Get(wstring key, Texture& ret)
{
    auto iter = mTexList.find(key);
    if (mTexList.end() == iter) {
        return false;
    }
    ret = *(iter->second);
	return true;
}

TextureManager::TextureManager() {}
TextureManager::~TextureManager()
{
    for (auto& data : mTexList)
    {
        data.second->Release();
        delete data.second;
    }
    mTexList.clear();
}