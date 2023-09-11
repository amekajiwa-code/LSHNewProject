#pragma once
#include "Std.h"

class Shader
{
	ID3D11VertexShader* m_pVS = nullptr;
	ID3D11PixelShader* m_pPS = nullptr;
	ID3D11HullShader* m_pHS = nullptr;
	ID3D11DomainShader* m_pDS = nullptr;
	ID3D11ComputeShader* m_pCS = nullptr;
	ID3D11GeometryShader* m_pGS = nullptr;
	ID3DBlob* m_VertexShaderCode = nullptr;

public:
	std::wstring   m_csName;
	std::wstring   m_csPath;
	LPVOID GetBufferPointer(void) const {
		return m_VertexShaderCode->GetBufferPointer();
	};
	SIZE_T GetBufferSize(void) const {
		return m_VertexShaderCode->GetBufferSize();
	};
	void Apply(ID3D11DeviceContext* pContext, int iSlot) const
	{
		pContext->VSSetShader(m_pVS, NULL, 0);
		pContext->PSSetShader(m_pPS, NULL, 0);
		pContext->HSSetShader(m_pHS, NULL, 0);
		pContext->DSSetShader(m_pDS, NULL, 0);
		pContext->CSSetShader(m_pCS, NULL, 0);
		pContext->GSSetShader(m_pGS, NULL, 0);
	}
	bool  LoadVertexShader(ID3D11Device* pDevice, wstring filename);
	bool  LoadPixelShader(ID3D11Device* pDevice, wstring filename);
	bool  Load(ID3D11Device* pDevice, wstring filename);
	bool  Release();
};
class ShaderManager
{
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}

	unordered_map<wstring, Shader*> m_list = unordered_map<wstring, Shader*>();
	void Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
	const Shader* Load(std::wstring szFilepath);
	const Shader* GetPtr(std::wstring key);
	bool	  Get(std::wstring key, Shader& ret);
	
private:
	ShaderManager();
public:
	virtual ~ShaderManager();
};