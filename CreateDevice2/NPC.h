#pragma once
#include "PlaneObject.h"

class Npc : public PlaneObject
{
	Vector3 m_vDirection = { 0,0,0 };
public:
	void Move(float fSecond);
	virtual bool Frame();

	Npc();
	virtual ~Npc();
};

