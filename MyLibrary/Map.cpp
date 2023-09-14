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
			m_VertexList[iVertexIndex].p.x = (col - mHalfCols) * mMapDesc.sellDistance;
			m_VertexList[iVertexIndex].p.z = -((row - mHalfRows) * mMapDesc.sellDistance);
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
	PlaneObject::Create(mMapDesc.textureFileName, mMapDesc.shaderFileName);
}