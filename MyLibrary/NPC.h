#pragma once
#include "PlaneObject.h"

enum class NpcState
{
	IDLE,
	RUN,
	ATTACK,
	DEAD,
};

class Npc : public PlaneObject
{
protected:
	Vector3 m_vDirection = { 0.0f, 0.0f, 0.0f };
	NpcState mNPCState = NpcState::IDLE;
	vector<pair<NpcState, const Texture*>> mAniList;
	int mAniCount = 0;
	float mAniTimer = 0.0f;
	Object* mTarget;
public:
	Npc();
	virtual ~Npc();

	NpcState GetNPCState();
	void SetNPCState(NpcState state);
	vector<const Texture*> GetAnimationList(NpcState state);
	void AddAnimationList(NpcState state, const Texture* texture);
	void SetTarget(Object* target);

	void Move();

	virtual bool DetectPlayer();
	virtual void Attack();

	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
};

