#pragma once
#include "PlaneObject.h"

class Floor : public PlaneObject
{
private:
	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

