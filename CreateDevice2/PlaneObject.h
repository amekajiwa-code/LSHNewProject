#pragma once
#include "Object.h"
class PlaneObject : public Object
{
public:
	virtual bool CreateVertexBuffer() override;
	virtual bool Frame();
};