#pragma once
#include "Window.h"

class Texture
{
private:
	ID3D11ShaderResourceView* mTexSRV = nullptr;
public:
	wstring mName;
	wstring mPath;

	void Apply(ID3D11DeviceContext* immediateContext, int slot) const
	{
		immediateContext->PSSetShaderResources(slot, 1, &mTexSRV);
	}
	bool Load(ID3D11Device* device, wstring fileName);
	bool Release();
};

class TextureManager
{
private:
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mImmediateContext = nullptr;
	unordered_map<wstring, Texture*> mTexList;
public:
    void Set(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	const Texture* Load(wstring filePath);
	const Texture* GetPtr(wstring key);
	bool Get(wstring key, Texture& ret);

	TextureManager();
	virtual ~TextureManager();
};
