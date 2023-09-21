#pragma once
#include "Std.h"

class Camera
{
protected:
	XMMATRIX mViewMatrix; //뷰행렬
	XMMATRIX mOrthonormalProjMatrix; //오소노말
	XMMATRIX mProjMatrix; //프로젝션(투영) 행렬
	XMVECTOR mCameraPos = XMVectorSet(0.0f, 10.0f, -7.5f, 0.0f);
	DWORD mWindowWidth;
	DWORD mWindowHeight;
public:
	XMVECTOR m_vLook;
	XMVECTOR m_vUp;
	XMVECTOR m_vSide;
	float m_fSpeed = 100.0f;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	bool Create(XMFLOAT3 pos, XMFLOAT2 size);
	void SetView(XMFLOAT3 eye, XMFLOAT3 lookAt, XMFLOAT3 up);
	void SetProjection(float FOV, float aspectRatio, float nearest, float farDistance); //FOV는 라디안 aspectRatio는 종횡비
	void UpdateVector();

	XMMATRIX GetViewMatrix()
	{
		return mViewMatrix;
	}
	XMMATRIX GetProjMatrix()
	{
		return mProjMatrix;
	}
};

