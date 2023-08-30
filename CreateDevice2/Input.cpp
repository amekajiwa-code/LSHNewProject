#include "Input.h"
#include "Writer.h"

Vector3 Input::GetWorldPos(Vector2 window, Vector3 camera)
{
    float fHalfWidth = window.mX / 2.0f;
    float fHalfHeight = window.mY / 2.0f;

    // client
    Vector3 clientMouse = { (float)mMousePos.x, (float)mMousePos.y , 0.0f };
    // world
    Vector2 WorldMouse = { camera.mX - fHalfWidth,
                               camera.mY + fHalfHeight };
    clientMouse.mX = WorldMouse.mX + clientMouse.mX;
    clientMouse.mY = WorldMouse.mY - clientMouse.mY;
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

    for (int ikey = 0; ikey < 256; ikey++)
    {
        SHORT s = GetAsyncKeyState(ikey);
        if (s & 0x8000) // 1000 0000 0000 0000 // s 값의 최상위 비트가 1이라면 키가 눌린 상태
        {
            if (mkeyState[ikey] == static_cast<DWORD>(KeyState::KEY_FREE)) // 전 상태가 FREE면
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_DOWN);
            }
            else // 전 상태가 FREE가 아니면 눌러져 있는 상태니까 HOLD
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_HOLD);
            }
        }
        else
        {
            if (mkeyState[ikey] == static_cast<DWORD>(KeyState::KEY_DOWN) ||
                mkeyState[ikey] == static_cast<DWORD>(KeyState::KEY_HOLD)) // 눌러져 있다가 뗄떼
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_UP);
            }
            else // 아무것도 안눌러져있었으면 FREE
            {
                mkeyState[ikey] = static_cast<DWORD>(KeyState::KEY_FREE);
            }
        }
    }

	return true;
}

bool Input::Render()
{
#ifdef _DEBUG
    std::wstring mousePos = L"Mouse : ";
    mousePos += std::to_wstring(mMousePos.x);
    mousePos += L",";
    mousePos += std::to_wstring(mMousePos.y);
    mousePos += L"\n";
    Writer::GetInstance().AddText(mousePos, 10, 50, { 1.0f, 1.0f, 1.0f, 1.0f });
#endif
	return true;
}

bool Input::Release()
{
	return false;
}