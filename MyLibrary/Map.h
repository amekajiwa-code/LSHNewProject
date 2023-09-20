#pragma once
#include "Object.h"

struct MapDesc
{
    UINT numCols;
    UINT numRows;
	float cellDistance;
	float scaleHeight;
	wstring textureFileName;
	wstring shaderFileName;
};

class Map : public Object
{
protected:
	MapDesc mMapDesc;
    UINT mNumFace;
	float mHalfRows;
	float mHalfCols;
    UINT numCellRows;
    UINT numCellCols;
public:
	virtual bool CreateVertexData() override;
	virtual bool CreateIndexData() override;
	void CreateMap(MapDesc desc);
public:
    //HeightMap ������ �߰�
    XMMATRIX m_matNormal;
    XMVECTOR m_vLightDir;
    int m_iNumFace;
    UINT m_iNumIndex;
    shared_ptr<int> m_pNormalLookupTable = make_shared<int>();
    shared_ptr<XMVECTOR> m_pFaceNormals = make_shared<XMVECTOR>();
    bool m_bStaticLight;
//public:
//    virtual bool		GenVertexNormal();
//    virtual void		UpdateIndexBuffer(ID3D11DeviceContext* pContext, DWORD* pdwIndexArray, int iFaceCount);
virtual void CalcVertexColor(XMVECTOR vLightDir);
//    virtual bool		CreateMap(MapDesc& MapDesc);
//    virtual bool		Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext);
//    virtual bool		Load(MapDesc& MapDesc);
//    //--------------------------------------------------------------------------------------
//    // ���� �븻 ���긦 ���� �ʱ�ȭ �Լ� �� ���̽� �븻 ���
//    //--------------------------------------------------------------------------------------	
//    virtual void		InitFaceNormals();
virtual void CalcFaceNormals();
virtual XMVECTOR ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
//    //--------------------------------------------------------------------------------------
//    // ���� �븻 ��� ���̺� ���� �� ���
//    //--------------------------------------------------------------------------------------	
//    virtual void		GenNormalLookupTable();
virtual void CalcPerVertexNormalsFastLookup();
//    virtual float		GetHeightmap(int row, int col);
//    virtual float		GetHeightOfVertex(UINT Index);
//    virtual XMVECTOR GetNormalOfVertex(UINT Index);
//    virtual XMVECTOR GetColorOfVertex(UINT Index);
//    virtual XMVECTOR GetTextureOfVertex(float fOffsetX, float fOffsetY);
//    virtual float		GetHeight(float fPosX, float fPosZ);
//    virtual float		Lerp(float fStart, float fEnd, float fTangent);
};

