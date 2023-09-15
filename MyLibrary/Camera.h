#pragma once
#include "Std.h"

class Camera
{
protected:
	DirectX::XMMATRIX mViewMatrix; //뷰행렬
	DirectX::XMMATRIX mOrthonormalProjMatrix; //오소노말
	DirectX::XMMATRIX mProjMatrix; //프로젝션(투영) 행렬
	XMVECTOR mCameraPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DWORD mWindowWidth;
	DWORD mWindowHeight;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	bool Create(XMFLOAT3 pos, XMFLOAT2 size);
	void SetView(XMFLOAT3 eye, XMFLOAT3 lookAt, XMFLOAT3 up);
	void SetProjection(float FOV, float aspectRatio, float nearest, float farDistance); //FOV는 라디안 aspectRatio는 종횡비
	XMMATRIX GetViewMatrix()
	{
		return mViewMatrix;
	}
	XMMATRIX GetProjMatrix()
	{
		return mProjMatrix;
	}
};

