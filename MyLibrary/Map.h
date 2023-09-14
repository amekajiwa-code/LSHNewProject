#pragma once
#include "Object.h"

struct MapDesc
{
	unsigned int numCols;
	unsigned int numRows;
	float sellDistance;
	wstring textureFileName;
	wstring shaderFileName;
};

class Map : public Object
{
private:
	MapDesc mMapDesc;
	unsigned int mNumFace;
	float mHalfRows;
	float mHalfCols;
	unsigned int numCellRows;
	unsigned int numCellCols;
	
public:
	virtual bool CreateVertexData() override;
	virtual bool CreateIndexData() override;
	void CreateMap(MapDesc desc);
};

