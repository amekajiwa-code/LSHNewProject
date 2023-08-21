#pragma once
#include "Core.h";
#include "PlaneObject.h"

class sample : public Core
{
	TextureManager mTexMg;
	ShaderManager mShaMg;
	ID3D11BlendState* mAlphaBlend = nullptr;

	Object* obj = nullptr;
	vector<Object*> mNPCs;

	Matrix mMatView;
	Matrix mMatOrthonormalProjection;
	Vector3 mCameraPos;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};