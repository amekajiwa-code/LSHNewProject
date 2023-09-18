#include "Input.h"
#include "Writer.h"

XMFLOAT3 Input::GetWorldPos(XMFLOAT2 window, XMFLOAT3 camera)
{
    float fHalfWidth = window.x / 2.0f;
    float fHalfHeight = window.y / 2.0f;

    // client
    XMFLOAT3 clientMouse = { (float)mMousePos.x, (float)mMousePos.y , 0.0f };
    // world
    XMFLOAT2 WorldMouse = { camera.x - fHalfWidth,
                               camera.y + fHalfHeight };
    clientMouse.x = WorldMouse.x + clientMouse.x;
    clientMouse.y = WorldMouse.y - clientMouse.y;
    return clientMouse;
}

bool Input::Init()
{
    ZeroMemory(&mkeyState, sizeof(mkeyState));
	return true;
}

bool Input::Frame()
{
    ::GetCursorPos(&mMousePos);
    ::ScreenToClient(g_hWnd, &mMousePos);

    float tempX = mMousePos.x - mBeforeMousePos.x;
    float tempY = mMousePos.y - mBeforeMousePos.y;
    mOffset = XMVectorSet(tempX, tempY, 0.0f, 0.0f);

    for (int ikey = 0; ikey < 256; ikey++)
    {
        SHORT s = GetAsyncKeyState(ikey);
        if (s & 0x8000) // 1000 0000 0000 0000 // s ���� �ֻ��� ��Ʈ�� 1�̶�� Ű�� ���� ����
        {
            if (mkeyState[ikey] == static_cast<DWORD>(KeyState::KEY_FREE)) // �� ���°� FREE��
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_DOWN);
            }
            else // �� ���°� FREE�� �ƴϸ� ������ �ִ� ���´ϱ� HOLD
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_HOLD);
            }
        }
        else
        {
            if (mkeyState[ikey] == static_cast<DWORD>(KeyState::KEY_DOWN) ||
                mkeyState[ikey] == static_cast<DWORD>(KeyState::KEY_HOLD)) // ������ �ִٰ� ����
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_UP);
            }
            else // �ƹ��͵� �ȴ������־����� FREE
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_FREE);
            }
        }
    }

    mBeforeMousePos = mMousePos;

	return true;
}

bool Input::Render()
{
	return true;
}

bool Input::Release()
{
	return false;
}