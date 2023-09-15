#pragma once
#include "Std.h"
//#include "sample.h"

enum class KeyState
{
	KEY_FREE,
	KEY_UP,
	KEY_DOWN,
	KEY_HOLD,
};

class Input
{
public:
	static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}

	XMVECTOR mOffset = g_XMZero; // vector 0으로 초기화
	POINT mMousePos;
	DWORD mkeyState[256];
	XMFLOAT3 curWorldPos;

	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	XMFLOAT3 GetWorldPos(XMFLOAT2 vWindow, XMFLOAT3 vCamera);

	virtual ~Input() {};
private:
	Input() {};
};

