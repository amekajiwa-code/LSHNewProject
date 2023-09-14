#pragma once
#include "Map.h"

class HeightMap : public Map
{
public:
	vector<float> mHeightList;

	bool CreateHeightMap(MapDesc desc);
	float GetHeightMap(int row, int col);
	float GetHeightOfVertex(unsigned int index);
	XMFLOAT4 GetColorOfVertex(unsigned int index);

	HeightMap();
	virtual ~HeightMap();
};

