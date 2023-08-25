#include "NPC.h"

void Npc::Move(float fSecond)
{
	Vector3 vVelocity = m_vDirection * 1000.0f * fSecond;
	m_vPos = m_vPos + vVelocity;

	if (m_vPos.mX < -static_cast<float>(g_dwWindowWidth) / 2)
	{
		m_vDirection.mX *= -1.0f;
		m_vPos.mX = -static_cast<float>(g_dwWindowWidth) / 2;
	}
	if (m_vPos.mY < -static_cast<float>(g_dwWindowHeight) / 2)
	{
		m_vDirection.mY *= -1.0f;
		m_vPos.mY = -static_cast<float>(g_dwWindowHeight) / 2;
	}
	if (m_vPos.mX > static_cast<float>(g_dwWindowWidth) / 2)
	{
		m_vDirection.mX *= -1.0f;
		m_vPos.mX = static_cast<float>(g_dwWindowWidth) / 2;
	}
	if (m_vPos.mY > static_cast<float>(g_dwWindowHeight) / 2)
	{
		m_vDirection.mY *= -1.0f;
		m_vPos.mY = static_cast<float>(g_dwWindowHeight) / 2;
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