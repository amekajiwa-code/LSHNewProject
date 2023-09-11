#include "Bullet.h"
#include "OBB.h"
#include "Timer.h"

bool Bullet::hitBullet(Object* target)
{
	OBB obb;
	if (target != nullptr && (target->GetInvincible() == false))
	{
		if (obb.RectToRect(PlaneObject::mRect, target->mRect))
		{
			target->m_bDead = true;
		}
	}

	return false;
}

void Bullet::fire(bool isShootLeft)
{
	Vector3 direction = { 1.0f, 0.0f, 0.0f };

	if (isShootLeft)
	{
		direction = { -1.0f, 0.0f, 0.0f };
	}

	Vector3 velocity = direction * Timer::GetInstance().mSecondPerFrame;
	m_vPos.mX = m_vPos.mX + velocity.mX;

	if (m_vPos.mX < -static_cast<float>(g_dwWindowWidth))
	{
		isEnable = false;
		m_vPos.mX = -static_cast<float>(g_dwWindowWidth);
	}
	if (m_vPos.mY < -static_cast<float>(g_dwWindowHeight))
	{
		isEnable = false;
		m_vPos.mY = -static_cast<float>(g_dwWindowHeight);
	}
	if (m_vPos.mX > static_cast<float>(g_dwWindowWidth))
	{
		isEnable = false;
		m_vPos.mX = static_cast<float>(g_dwWindowWidth);
	}
	if (m_vPos.mY > static_cast<float>(g_dwWindowHeight))
	{
		isEnable = false;
		m_vPos.mY = static_cast<float>(g_dwWindowHeight);
	}
}