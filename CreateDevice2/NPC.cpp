#include "NPC.h"

void Npc::Move(float fSecond)
{
	Vector3 vVelocity = m_vDirection * 500.0f * fSecond;
	m_vPos = m_vPos + vVelocity;

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

bool Npc::Frame()
{
	Matrix matScale, matRotation, matTranslate;
	matScale.Scale(m_vScale);
	matRotation.ZRotate(m_vRotation.mZ);
	matTranslate.Translation(m_vPos);
	m_matWorld = matScale * matRotation * matTranslate;
	return true;
}

Npc::Npc()
{
	m_vDirection = { randstep(-1, +1), randstep(-1, +1), 0 };
}

Npc::~Npc()
{
	//something cool code~!
}