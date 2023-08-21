#pragma once
#include "Window.h"

class Shader
{
private:
	ID3D11VertexShader* mVS = nullptr;
	ID3D11PixelShader* mPS = nullptr;

	/*ID3D11HullShader* m_pHS = nullptr;
	ID3D11DomainShader* m_pDS = nullptr;
	ID3D11ComputeShader* m_pCS = nullptr;
	ID3D11GeometryShader* m_pGS = nullptr;*/
public:
	ID3DBlob* mVertexShaderCode = nullptr;
	wstring mName;
	wstring mPath;

	LPVOID GetBufferPointer(void) const
	{
		return mVertexShaderCode->GetBufferPointer();
	}

	SIZE_T GetBufferSize(void) const
	{
		return mVertexShaderCode->GetBufferSize();
	}

	void Apply(ID3D11DeviceContext* context, int slot) const
	{
		context->VSSetShader(mVS, NULL, 0);
		context->PSSetShader(mPS, NULL, 0);
	}

	bool LoadVertexShader(ID3D11Device* device, wstring fileName);
	bool LoadPixelShader(ID3D11Device* device, wstring fileName);
	bool Load(ID3D11Device* device, wstring fileName);
	bool Release();
};

class ShaderManager
{
private:
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mImmediateContext = nullptr;
	unordered_map<wstring, Shader*> mShaList;
public:
	void Set(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	const Shader* Load(wstring filePath);
	const Shader* GetPtr(wstring key);
	bool Get(wstring key, Shader& ret);

	ShaderManager();
	virtual ~ShaderManager();
};

