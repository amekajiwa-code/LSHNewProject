#pragma once
#include "Std.h"

class Window
{
public:
    bool SetRegisterClassWindow(HINSTANCE hInstance);
    bool SetWindow(const WCHAR* szTitle, DWORD dwWindowWidth, DWORD dwWindowHeight);

    HINSTANCE m_hInstance;
    HWND m_hWnd;
    DWORD m_dwExStyle = 0;
    DWORD m_dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD m_dwWindowPosX = 0;
    DWORD m_dwWindowPosY = 0;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;

    virtual ~Window() {}
private:
};