#pragma once
#include "Std.h"

class Texture
{
	ID3D11ShaderResourceView* m_pTexSRV = nullptr;
public:
	std::wstring   m_csName;
	std::wstring   m_csPath;
	void Apply(ID3D11DeviceContext* pImmediateContext, int iSlot) const
	{
		pImmediateContext->PSSetShaderResources(iSlot, 1, &m_pTexSRV);
	}
	bool  Load(ID3D11Device* pDevice, wstring filename);
	bool  Release();
};
class TextureManager
{
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	unordered_map<wstring, Texture*> m_list = unordered_map<wstring, Texture*>();
	void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
	const Texture* Load(std::wstring szFilepath);
	const Texture* GetPtr(std::wstring key);
	bool	  Get(std::wstring key, Texture& ret);

private:
	TextureManager();
public:
	virtual ~TextureManager();
};
