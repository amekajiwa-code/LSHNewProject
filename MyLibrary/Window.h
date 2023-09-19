#pragma once
#include "Std.h"

class Window
{
public:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    DWORD m_dwExStyle = 0;
    DWORD m_dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD m_dwWindowPosX = 0;
    DWORD m_dwWindowPosY = 0;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;
    RECT mRectClient;

    bool SetRegisterClassWindow(HINSTANCE hInstance);
    bool SetWindow(const WCHAR* szTitle, DWORD dwWindowWidth, DWORD dwWindowHeight);
    virtual int MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual void ResizeDevice(UINT width, UINT height) {};

    Window();
    virtual ~Window() {}
private:
};