#pragma once
#include "Std.h"
//#include "sample.h"

enum class KeyState
{
	KEY_FREE = 0,
	KEY_UP = 1,
	KEY_DOWN = 2,
	KEY_HOLD = 3,
};

class Input
{
public:
	static Input& GetInstance()
	{
		static Input instance;
		return instance;
	}

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

