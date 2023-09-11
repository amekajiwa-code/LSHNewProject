#pragma once
#include "PlaneObject.h"

class Bullet : public PlaneObject
{
public:
	bool isEnable = true;

	bool hitBullet(Object* target);
	void fire(bool isShootLeft);
};

