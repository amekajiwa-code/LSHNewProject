#include "Floor.h"

bool Floor::Init()
{
	PlaneObject::mTag = "Floor";
	return true;
}

bool Floor::Frame()
{
	return false;
}

bool Floor::Render()
{
	PlaneObject::Render();
	return true;
}

bool Floor::Release()
{
	return false;
}