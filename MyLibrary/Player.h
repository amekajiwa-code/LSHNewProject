#pragma once
#include "PlaneObject.h"
#include "SoundManager.h"

enum class PlayerState
{
	IDLE,
	RUN,
	JUMP,
	FALL,
	ATTACK,
	DEAD,
};

class Player : public PlaneObject
{
private:
	//행동 관련
	const float MAX_JUMP_TIME = 0.3f;
	float mJumpTimer = 0.0f;
	const float MAX_ATTACK_TIME = 0.2f;
	float mAttackTimer = 0.0f;
	const float MAX_DELAY_TIME = 0.2f;
	float mDelayTimer = 0.0f;
	const float mSpeed = 600.0f;
	const float mPower = 800;
	const float mGrabity = 9.81f * 50.0f;
	Vector3 mVelocity;
	bool isFloor;
	bool isJump;
	bool isFlipY;
	float lastFloorY = -300.0f;
	
	PlayerState mPlayerState;
	vector<pair<PlayerState, const Texture*>> mAniList;
	int mAniCount = 0;
	float mAniTimer = 0.0f;
	float maxAniTime = 0.2f;
	Vector3 mTargetDirection;
	float mRadianAngle = 0.0f;
public:
	//사운드
	Sound* mJumpSound = SoundManager::GetInstance().Load(L"res/sound/player_jump.wav");
	Sound* mLandSound = SoundManager::GetInstance().Load(L"res/sound/player_land.wav");
	bool isLandSound;
	Sound* mDashSound = SoundManager::GetInstance().Load(L"res/sound/player_prerun.wav");
	bool isDashSound;
	float mDashTimer = 0.0f;
	const float MAX_Dash_TIME = 0.3f;
	Sound* mRunSound = SoundManager::GetInstance().Load(L"res/sound/player_running_2.wav");
	Sound* mEffectSound = SoundManager::GetInstance().Load(L"res/sound/slash_1.wav");
	//이펙트 스프라이트
public:
	void PlayerMove();
	void MoveX(bool isFlipY);
	bool CheckCollision(Object* other);
	void AddAnimationList(PlayerState state, const Texture* texture);
	vector<const Texture*> GetAnimationList(PlayerState state);
	void PlayerAttack();

	PlayerState GetPlayerState()
	{
		return mPlayerState;
	}
	void SetPlayerState(PlayerState state)
	{
		mPlayerState = state;
	}

	void SetDirection(Vector3 direction)
	{
		mTargetDirection = direction;
	}

	float GetRadianAngle()
	{
		return mRadianAngle;
	}

	void SetRadianAngle(float angle)
	{
		mRadianAngle = angle;
	}

	bool GetFlip() { return isFlipY; }
	void SetFlip(bool flip) { isFlipY = flip; }

	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

