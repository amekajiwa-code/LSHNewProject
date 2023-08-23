#pragma once
#include "PlaneObject.h"

class Player : public PlaneObject
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

