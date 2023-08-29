#pragma once
#include "PlaneObject.h"

enum class PlayerState
{
	IDLE,
	RUN,
	JUMP,
	FALL,
	ATTACK,
};

class Player : public PlaneObject
{
private:
	float MAX_JUMP_TIME = 0.3f;
	bool isFloor;
	bool isJump;
	bool isFlipY;
	float mJumpTimer;
	PlayerState mPlayerState = PlayerState::IDLE;

	void PlayerMove();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

