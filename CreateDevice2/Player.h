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
	float mJumpTimer = 0.0f;
	float MAX_ATTACK_TIME = 0.3f;
	float mAttackTimer = 0.0f;
	bool isFloor;
	bool isJump;
	bool isFlipY;
	PlayerState mPlayerState;

	void PlayerMove();
public:
	vector<const Texture*>  mIdleList = vector<const Texture*>();
	vector<const Texture*>  mRunList = vector<const Texture*>();
	vector<const Texture*>  mJumpList = vector<const Texture*>();
	vector<const Texture*>  mFallList = vector<const Texture*>();
	vector<const Texture*>  mAttackList = vector<const Texture*>();

	//vector<std::pair<PlayerState, const Texture*>>; <---수정예정

	bool CheckCollision(Object* other);
	vector<const Texture*> GetPlayerAnimation();
	void PlayerAttack();

	PlayerState GetPlayerState()
	{
		return mPlayerState;
	}
	void SetPlayerState(PlayerState state)
	{
		mPlayerState = state;
	}

	bool GetFlip() { return isFlipY; }

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

