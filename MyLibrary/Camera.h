#pragma once
#include "Std.h"

class Camera
{
private:
	DirectX::XMMATRIX mViewMatrix; //�����
	DirectX::XMMATRIX mOrthonormalProjMatrix; //���ҳ븻
	DirectX::XMMATRIX mProjMatrix; //��������(����) ���
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
	void SetdProjection(float FOV, float aspectRatio, float nearest, float farDistance); //FOV�� ���� aspectRatio�� ��Ⱦ��
	XMMATRIX GetViewMatrix()
	{
		return mViewMatrix;
	}
	XMMATRIX GetProjMatrix()
	{
		return mProjMatrix;
	}
};

