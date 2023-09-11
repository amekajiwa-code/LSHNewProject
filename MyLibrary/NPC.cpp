#include "NPC.h"
#include "Timer.h"
#include "cmath"
#include "OBB.h"

void Npc::Move()
{
	m_vDirection = mTarget->m_vPos - PlaneObject::m_vPos;
	Vector3 vVelocity = m_vDirection * Timer::GetInstance().mSecondPerFrame;
	m_vPos.mX = m_vPos.mX + vVelocity.mX;

	if (m_vPos.mX < -static_cast<float>(g_dwWindowWidth))
	{
		m_vDirection.mX *= -1.0f;
		m_vPos.mX = -static_cast<float>(g_dwWindowWidth);
	}
	if (m_vPos.mY < -static_cast<float>(g_dwWindowHeight))
	{
		m_vDirection.mY *= -1.0f;
		m_vPos.mY = -static_cast<float>(g_dwWindowHeight);
	}
	if (m_vPos.mX > static_cast<float>(g_dwWindowWidth))
	{
		m_vDirection.mX *= -1.0f;
		m_vPos.mX = static_cast<float>(g_dwWindowWidth);
	}
	if (m_vPos.mY > static_cast<float>(g_dwWindowHeight))
	{
		m_vDirection.mY *= -1.0f;
		m_vPos.mY = static_cast<float>(g_dwWindowHeight);
	}

};

void Npc::Attack()
{
	OBB obb;
	if (mTarget != nullptr && (mTarget->GetInvincible() == false))
	{
		if (obb.RectToRect(PlaneObject::mRect, mTarget->mRect))
		{
			mTarget->m_bDead = true;
		}
	}
}

bool Npc::Init()
{
	return true;
}

bool Npc::Frame()
{
	switch(mNPCState)
	{
	case NpcState::IDLE:
		PlaneObject::SetScale({ 30.0f, 36.0f, 1.0f });
		DetectPlayer();
		break;
	case NpcState::RUN:
		PlaneObject::SetScale({ 36.0f, 39.0f, 1.0f });
		Move();
		DetectPlayer();
		break;
	case NpcState::ATTACK:
		PlaneObject::SetScale({ 44.0f, 42.0f, 1.0f });
		Attack();
		DetectPlayer();
		break;
	case NpcState::DEAD:
		PlaneObject::SetScale({ 56.0f, 41.0f, 1.0f });
		break;
	default:
		break;
	}

	Matrix matScale, matRotation, matTranslate;
	matScale.Scale(m_vScale);
	matRotation.ZRotate(m_vRotation.mZ);
	matTranslate.Translation(m_vPos);
	m_matWorld = matScale * matRotation * matTranslate;

	return true;
}

bool Npc::Render()
{
	PlaneObject::PreRender();

	if (!mAniList.empty())
	{
		vector<const Texture*> texList = GetAnimationList(mNPCState);
		{
			if (mAniTimer < 0.1f)
			{
				mAniTimer += Timer::GetInstance().mSecondPerFrame;
			}
			else
			{
				++mAniCount;
				mAniTimer = 0.0f;
			}

			if (mAniCount >= texList.size() && mNPCState == NpcState::DEAD)
			{
				--mAniCount;
			}
			else if (mAniCount >= texList.size())
			{
				mAniCount = 0;
			}

			texList[mAniCount]->Apply(m_pImmediateContext, 0);
		}
	}
	PlaneObject::PostRender();

	return true;
}


bool Npc::Release()
{
	return false;
}

NpcState Npc::GetNPCState()
{
	return mNPCState;
}

void Npc::SetNPCState(NpcState state)
{
	mNPCState = state;
}

vector<const Texture*> Npc::GetAnimationList(NpcState state)
{	
	vector<const Texture*> texList;

	for (auto it = mAniList.begin(); it != mAniList.end(); ++it)
	{
		if (it->first == state) {
			texList.push_back(it->second);
		}
	}

	return texList;
}

void Npc::AddAnimationList(NpcState state, const Texture* texture)
{
	mAniList.push_back(make_pair(state, texture));
}

bool Npc::DetectPlayer()
{
	float range = PlaneObject::m_vPos.mX - mTarget->m_vPos.mX;

	if (range > 0)
	{
		m_VertexList[0].t.mX = 1.0f; m_VertexList[0].t.mY = 0.0f;
		m_VertexList[1].t.mX = 0.0f; m_VertexList[1].t.mY = 0.0f;
		m_VertexList[2].t.mX = 1.0f; m_VertexList[2].t.mY = 1.0f;
		m_VertexList[3].t.mX = 1.0f; m_VertexList[3].t.mY = 1.0f;
		m_VertexList[4].t.mX = 0.0f; m_VertexList[4].t.mY = 0.0f;
		m_VertexList[5].t.mX = 0.0f; m_VertexList[5].t.mY = 1.0f;
	}
	else
	{
		m_VertexList[0].t.mX = 0.0f; m_VertexList[0].t.mY = 0.0f;
		m_VertexList[1].t.mX = 1.0f; m_VertexList[1].t.mY = 0.0f;
		m_VertexList[2].t.mX = 0.0f; m_VertexList[2].t.mY = 1.0f;
		m_VertexList[3].t.mX = 0.0f; m_VertexList[3].t.mY = 1.0f;
		m_VertexList[4].t.mX = 1.0f; m_VertexList[4].t.mY = 0.0f;
		m_VertexList[5].t.mX = 1.0f; m_VertexList[5].t.mY = 1.0f;
	}
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, &m_VertexList.at(0), 0, 0);

	if (fabs(range) < 100.0f && mNPCState != NpcState::DEAD)
	{
		mNPCState = NpcState::ATTACK;
		
		return true;
	}
	else if (fabs(range) < 300.0f && mNPCState != NpcState::DEAD)
	{
		mNPCState = NpcState::RUN;
		
		return true;
	}
	else
	{
		mNPCState = NpcState::IDLE;
		
		return false;
	}
}

void Npc::SetTarget(Object* target)
{
	mTarget = target;
}

Npc::Npc()
{
	m_vDirection = { randstep(-1, +1), randstep(-1, +1), 0 };
}

Npc::~Npc()
{
	//something cool code~!
}