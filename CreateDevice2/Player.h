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
	vector<const Texture*>  mIdleList;
	vector<const Texture*>  mRunList;
	vector<const Texture*>  mJumpList;
	vector<const Texture*>  mFallList;
	vector<const Texture*>  mAttackList;

	void PlayerMove();
	void PlayerAttack();
	bool CheckCollision(Object* other);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

