#pragma once
#include "GeometryUtils.h"

class MyCollision
{
public:
	MyCollision() { }

	bool CheckCollision(const Box& box1, const Box& box2);
};

