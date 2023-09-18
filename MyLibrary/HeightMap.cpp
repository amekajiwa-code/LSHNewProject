#include "HeightMap.h"

bool HeightMap::CreateHeightMap(MapDesc desc)
{
	Map::CreateMap(desc);
	ComPtr<ID3D11Resource> pTexture;
	size_t maxSize = 0;
	ID3D11Texture2D* pTexture2D = NULL;
	HRESULT hr = pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D);

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

float HeightMap::GetHeightMap(int row, int col)
{
	return mHeightList[row * mMapDesc.numRows + col] * mMapDesc.scaleHeight;
}

float HeightMap::GetHeightOfVertex(unsigned int index)
{
	return mHeightList[index] * mMapDesc.scaleHeight;
}

XMFLOAT4 HeightMap::GetColorOfVertex(unsigned int index)
{
	return XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

HeightMap::HeightMap() {}
HeightMap::~HeightMap() {}