#pragma once
#include "PlaneObject.h"

class Npc : public PlaneObject
{
	Vector3 m_vDirection = { 0,0,0 };
public:
	void Move(float fSecond);
	virtual bool Frame();

	Npc()
	{
		m_vDirection = { randstep(-1, +1), randstep(-1, +1), 0 };
	}
};

