#pragma once
#include "Std.h"

class Camera
{
public:
	Matrix mMatView;
	Matrix mMatOrthonormalProjection;
	Vector3 mCameraPos = { 0, 0, 0 };
	DWORD mWindowWidth;
	DWORD mWindowHeight;

	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool Create(Vector3 pos, Vector2 size);
};

