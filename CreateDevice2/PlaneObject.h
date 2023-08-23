#pragma once
#include "Object.h"
class PlaneObject : public Object
{
public:
	virtual bool CreateVertexBuffer() override;
	virtual bool    Frame();
};
class Npc : public PlaneObject
{
	Vector3 m_vDirection = { 1,1,0 };
public:
	void			Move(float fSecond)
	{
		Vector3 vVelocity = m_vDirection * 500.0f * fSecond;
		m_vPos = m_vPos + vVelocity;

		if (m_vPos.mX < -400.0f)
		{
			m_vDirection.mX *= -1.0f;
			m_vPos.mX = -400.0f;
		}
		if (m_vPos.mY < -300.0f)
		{
			m_vDirection.mY *= -1.0f;
			m_vPos.mY = -300.0f;
		}
		if (m_vPos.mX > 400.0f)
		{
			m_vDirection.mX *= -1.0f;
			m_vPos.mX = 400.0f;
		}
		if (m_vPos.mY > 300.0f)
		{
			m_vDirection.mY *= -1.0f;
			m_vPos.mY = 300.0f;
		}

	};
	virtual bool    Frame()
	{
		Matrix matScale, matRotation, matTranslate;
		matScale.Scale(m_vScale);
		matRotation.ZRotate(m_vRotation.mZ);
		matTranslate.Translation(m_vPos);
		m_matWorld = matScale * matRotation * matTranslate;
		return true;
	}

	Npc()
	{
		m_vDirection = { randstep(-1, +1), randstep(-1, +1), 0 };
	}
};