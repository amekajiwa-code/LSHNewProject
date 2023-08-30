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
	float mJumpTimer = 0.0f;
	PlayerState mPlayerState;

	void PlayerMove();
	void PlayerAttack();
public:
	vector<const Texture*>  mIdleList = vector<const Texture*>();
	vector<const Texture*>  mRunList = vector<const Texture*>();
	vector<const Texture*>  mJumpList = vector<const Texture*>();
	vector<const Texture*>  mFallList = vector<const Texture*>();
	vector<const Texture*>  mAttackList = vector<const Texture*>();

	bool CheckCollision(Object* other);
	vector<const Texture*> GetPlayerAnimation();

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

