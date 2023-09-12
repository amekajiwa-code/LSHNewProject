#pragma once
#include "Std.h"

class Camera
{
private:
	DirectX::XMMATRIX mViewMatrix; //뷰행렬
	DirectX::XMMATRIX mOrthonormalProjMatrix; //오소노말
	DirectX::XMMATRIX mProjMatrix; //프로젝션(투영) 행렬
	XMFLOAT3 mCameraPos = { 0, 0, 0 };
	DWORD mWindowWidth;
	DWORD mWindowHeight;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool Create(XMFLOAT3 pos, XMFLOAT2 size);
	void SetView(XMFLOAT3 eye, XMFLOAT3 lookAt, XMFLOAT3 up);
	void SetdProjection(float FOV, float aspectRatio, float nearest, float farDistance); //FOV는 라디안 aspectRatio는 종횡비
	XMMATRIX GetViewMatrix()
	{
		return mViewMatrix;
	}
	XMMATRIX GetProjMatrix()
	{
		return mProjMatrix;
	}
};

