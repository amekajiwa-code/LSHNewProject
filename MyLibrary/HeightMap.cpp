#include "HeightMap.h"

bool HeightMap::CreateHeightMap(MapDesc mapDesc)
{
	Map::CreateMap(mapDesc);
	ComPtr<ID3D11Resource> pTexture;
	size_t maxSize = 0;
	ID3D11Texture2D* pTexture2D = NULL;
	HRESULT hr = pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D);

	if (FAILED(hr))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	mHeightList.resize(desc.Height * desc.Width);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		hr = m_pImmediateContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest);
		if (SUCCEEDED(hr))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_Vertex	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					mHeightList[row * desc.Width + col] = (float)uRed;	/// DWORDÀÌ¹Ç·Î pitch/4	
				}
			}
			m_pImmediateContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	mapDesc.numRows = desc.Height;
	mapDesc.numCols = desc.Width;

	if (pTexture2D) pTexture2D->Release();

	return true;
}

float HeightMap::GetHeightMap(int row, int col)
{
	return mHeightList[row * mMapDesc.numRows + col] * mMapDesc.scaleHeight;
}

float HeightMap::GetHeightOfVertex(UINT index)
{
	return mHeightList[index] * mMapDesc.scaleHeight;
}

XMFLOAT4 HeightMap::GetColorOfVertex(UINT index)
{
	return XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

HeightMap::HeightMap() {}
HeightMap::~HeightMap() {}