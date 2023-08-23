#pragma once
#include "Std.h"

class Camera
{
public:
	Matrix mMatView;
	Matrix mMatOrthonormalProjection;
	Vector3 mCameraPos = { 0, 0, 0 };
	DWORD mWindowWidth = 800;
	DWORD mWindowHeight = 600;

	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool Create(Vector3 pos, Vector2 size);
};

