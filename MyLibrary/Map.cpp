#include "Map.h"

bool Map::CreateVertexData()
{
	m_VertexList.resize(mMapDesc.numCols * mMapDesc.numRows);

	mHalfCols = (mMapDesc.numCols - 1) * 0.5f;
	mHalfRows = (mMapDesc.numRows - 1) * 0.5f;

	for (int row = 0; row < mMapDesc.numRows; ++row)
	{
		for (int col = 0; col < mMapDesc.numCols; ++col)
		{
			int  iVertexIndex = row * mMapDesc.numCols + col;
			m_VertexList[iVertexIndex].p.x = (col - mHalfCols) * mMapDesc.cellDistance;
			m_VertexList[iVertexIndex].p.z = -((row - mHalfRows) * mMapDesc.cellDistance);
			m_VertexList[iVertexIndex].p.y = 0.0f;
			XMFLOAT3 xmfloat3 = { 0.0f, 1.0f, 0.0f };
			m_VertexList[iVertexIndex].n = xmfloat3;
			XMFLOAT4 xmfloat4 = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_VertexList[iVertexIndex].c = xmfloat4;
			float u = (float)col / (float)(mMapDesc.numCols - 1);
			float v = (float)row / (float)(mMapDesc.numRows - 1);
			XMFLOAT2 xmfloat2 = { u , v };
			m_VertexList[iVertexIndex].t = xmfloat2;
		}
	}
	return true;
}

bool Map::CreateIndexData()
{
	numCellRows = mMapDesc.numRows - 1;
	numCellCols = mMapDesc.numCols - 1;
	mNumFace = numCellRows * numCellCols * 2;
	mIndexList.resize(mNumFace * 3);

	int curIndex = 0;
	for (int row = 0; row < numCellRows; ++row)
	{
		for (int col = 0; col < numCellCols; ++col)
		{
			int NextRow = row + 1;
			int NextCol = col + 1;
			mIndexList[curIndex + 0] = row * mMapDesc.numCols + col;
			mIndexList[curIndex + 1] = row * mMapDesc.numCols + NextCol;
			mIndexList[curIndex + 2] = NextRow * mMapDesc.numCols + col;
			mIndexList[curIndex + 3] = mIndexList[curIndex + 2];
			mIndexList[curIndex + 4] = mIndexList[curIndex + 1];
			mIndexList[curIndex + 5] = NextRow * mMapDesc.numCols + NextCol;

			curIndex += 6;
		}
	}
	return true;
}

void Map::CreateMap(MapDesc desc)
{
	mMapDesc = desc;
	Object::Create(mMapDesc.textureFileName, mMapDesc.shaderFileName);
}

//Height Map 때문에 추가
//bool Map::GenVertexNormal()
//{
//	///////////////////////////////////////////////////////////////////
//	//// 페이스 노말 계산 및  이웃 페이스 인덱스 저장하여 정점 노말 계산
//	///////////////////////////////////////////////////////////////////
//	InitFaceNormals();
//	GenNormalLookupTable();
//	CalcPerVertexNormalsFastLookup();
//	return true;
//}
//
//void Map::UpdateIndexBuffer(ID3D11DeviceContext* pContext, DWORD* pdwIndexArray, int iFaceCount)
//{
//}

void Map::CalcVertexColor(XMVECTOR vLightDir)
{
	/////////////////////////////////////////////////////////////////
	// 페이스 노말 계산 및  이웃 페이스 인덱스 저장하여 정점 노말 계산
	/////////////////////////////////////////////////////////////////	
	for (int iRow = 0; iRow < mMapDesc.numRows; iRow++)
	{
		for (int iCol = 0; iCol < mMapDesc.numCols; iCol++)
		{
			//<vLightDir = 0,-1.0f,0>
			int  iVertexIndex = iRow * mMapDesc.numCols + iCol;
			//D3DXVec3Normalize(	&m_VertexList[iVertexIndex].n, 
			//					&m_VertexList[iVertexIndex].n);
			XMVECTOR vertexVec = XMLoadFloat3(&m_VertexList[iVertexIndex].n);
			XMVECTOR fDot = XMVector3Dot(-vLightDir, vertexVec);
			XMVECTOR colorVec = XMLoadFloat4(&m_VertexList[iVertexIndex].c) * fDot;
			XMStoreFloat4(&m_VertexList[iVertexIndex].c, colorVec);
			m_VertexList[iVertexIndex].c.w = 1.0f;
		}
	}
}

//bool Map::CreateMap(MapDesc& MapDesc)
//{
//
//}
//
//bool Map::Init(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext)
//{
//
//}
//
//bool Map::Load(MapDesc& MapDesc)
//{
//
//}
////--------------------------------------------------------------------------------------
//// 정점 노말 연산를 위한 초기화 함수 및 페이스 노말 계산
////--------------------------------------------------------------------------------------	
//void Map::InitFaceNormals()
//{
//
//}

void Map::CalcFaceNormals()
{
	// Loop over how many faces there are
	int j = 0;
	for (int i = 0; i < m_iNumFace * 3; i += 3)
	{
		DWORD i0 = mIndexList[i];
		DWORD i1 = mIndexList[i + 1];
		DWORD i2 = mIndexList[i + 2];
		m_pFaceNormals[j] = ComputeFaceNormal(i0, i1, i2);
		j++;
	}
}

XMVECTOR Map::ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2)
{
	XMVECTOR vNormal;
	XMVECTOR v0 = XMLoadFloat3(&m_VertexList[dwIndex1].p) - XMLoadFloat3(&m_VertexList[dwIndex0].p);
	XMVECTOR v1 = XMLoadFloat3(&m_VertexList[dwIndex2].p) - XMLoadFloat3(&m_VertexList[dwIndex0].p);

	vNormal = XMVector3Cross(v0, v1);
	vNormal = XMVector3Normalize(vNormal);
	return vNormal;
}
////--------------------------------------------------------------------------------------
//// 정점 노말 룩업 테이블 생성 및 계산
////--------------------------------------------------------------------------------------	
//void Map::GenNormalLookupTable()
//{
//
//}

void Map::CalcPerVertexNormalsFastLookup()
{
	// First, calculate the face normals for each triangle.
	CalcFaceNormals();

	// For each vertex, sum the normals indexed by the normal lookup table.
	int j = 0;
	for (int i = 0; i < m_iNumVertices; i++)
	{
		TVector3 avgNormal;
		avgNormal = TVector3(0.0f, 0.0f, 0.0f);

		// Find all the triangles that this vertex is a part of.
		for (j = 0; j < 6; j++)
		{
			int triIndex;
			triIndex = m_pNormalLookupTable[i * 6 + j];

			// If the triangle index is valid, get the normal and average it in.
			if (triIndex != -1)
			{
				avgNormal += m_pFaceNormals[triIndex];
			}
			else
				break;
		}

		// Complete the averaging step by dividing & normalizing.
		_ASSERT(j > 0);
		avgNormal.x /= (float)j;//.DivConst( (float)(j) );
		avgNormal.y /= (float)j;
		avgNormal.z /= (float)j;
		D3DXVec3Normalize(&avgNormal, &avgNormal);

		// Set the vertex normal to this new normal.
		m_VertexList[i].n.x = avgNormal.x;
		m_VertexList[i].n.y = avgNormal.y;
		m_VertexList[i].n.z = avgNormal.z;

	}  // For each vertex

	/////////////////////////////////////////////////////////////////
	// 페이스 노말 계산 및  이웃 페이스 인덱스 저장하여 정점 노말 계산
	/////////////////////////////////////////////////////////////////	
	if (m_bStaticLight) CalcVertexColor(m_vLightDir);
}

//float Map::GetHeightmap(int row, int col)
//{
//
//}
//
//float Map::GetHeightOfVertex(UINT Index)
//{
//
//}
//
//XMVECTOR Map::GetNormalOfVertex(UINT Index)
//{
//
//}
//
//XMVECTOR Map::GetColorOfVertex(UINT Index)
//{
//
//}
//
//XMVECTOR Map::GetTextureOfVertex(float fOffsetX, float fOffsetY)
//{
//
//}
//
//float Map::GetHeight(float fPosX, float fPosZ)
//{
//
//}
//
//float Map::Lerp(float fStart, float fEnd, float fTangent)
//{
//
//}