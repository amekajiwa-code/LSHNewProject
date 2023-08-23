#pragma once
#include "sample.h"

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

	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	virtual ~Input() {};
private:
	Input() {};
};

