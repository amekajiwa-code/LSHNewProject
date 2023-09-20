#pragma once
#include "Map.h"

class HeightMap : public Map
{
public:
	vector<float> mHeightList;

	bool CreateHeightMap(MapDesc desc);
	float GetHeightOfVertex(UINT index);
	XMFLOAT4 GetColorOfVertex(UINT index);
	float GetHeightMap(int row, int col);

	HeightMap();
	virtual ~HeightMap();
};

