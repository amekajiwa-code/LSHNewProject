#pragma once
#include "Object.h"

class PlaneObject : public Object
{
public:
	virtual bool CreateVertexBuffer() override;
	virtual bool Frame();
};

class NPC : public PlaneObject
{
private:
	Vector3 mDirection = { 0, 0, 0 };
public:
	NPC()
	{
		mDirection = { (float)randstep(-1, 1), (float)randstep(-1, 1), 0 };
	}

	void Move(float second)
	{
		Vector3 velocity = mDirection * 500.0f * second;
		mPos = mPos + velocity;

		// 화면 경계에 도달하면 이동 방향을 반대로 바꾸어 반사되도록 합니다.
		if (mPos.mX < 0.0f || mPos.mX > 800.0f) {
			mDirection.mX *= -1.0f;
		}
		if (mPos.mY < 0.0f || mPos.mY > 600.0f) {
			mDirection.mY *= -1.0f;
		}
	}

	virtual bool Frame()
	{
		Matrix matScale, matRotation, matTranslate; // SRT
		matScale.Scale(mScale);
		matRotation.ZRotate(mRotation.mZ);
		matTranslate.Translation(mPos);
		mMatWorld = matScale * matRotation * matTranslate;
		return true;
	}
};

