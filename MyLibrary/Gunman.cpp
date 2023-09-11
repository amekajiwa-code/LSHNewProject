#include "Gunman.h"
#include "OBB.h"

bool Gunman::Frame()
{
	switch (mNPCState)
	{
	case NpcState::IDLE:
		PlaneObject::SetScale({ 49.0f, 50.0f, 1.0f });
		DetectPlayer();
		break;
	case NpcState::RUN:
		PlaneObject::SetScale({ 45.0f, 40.0f, 1.0f });
		Move();
		DetectPlayer();
		break;
	case NpcState::ATTACK:
		PlaneObject::SetScale({ 48.0f, 35.0f, 1.0f });
		Attack();
		DetectPlayer();
		break;
	case NpcState::DEAD:
		PlaneObject::SetScale({ 44.0f, 34.0f, 1.0f });
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

bool Gunman::DetectPlayer()
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

void Gunman::Attack()
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