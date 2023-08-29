#pragma once
#include "PlaneObject.h"

class Player : public PlaneObject
{
private:
	bool isFloor = false;
	bool isJump = false;
	bool isFlipY = false;
	float MAX_JUMP_TIME = 1.0f;
	float mJumpTimer;

	void PlayerMove();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

