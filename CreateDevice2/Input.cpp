#include "Input.h"

bool Input::Init()
{
    ZeroMemory(&mkeyState, sizeof(mkeyState));
	return true;
}

bool Input::Frame()
{
    ::GetCursorPos(&mMousePos);
    ::ScreenToClient(g_hWnd, &mMousePos);

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

	return true;
}

bool Input::Render()
{
	return false;
}

bool Input::Release()
{
	return false;
}