#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // 윈도우 프로시저
{
    switch (message) //메시지에 따라 동작
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool Window::SetRegisterWindow(HINSTANCE hInstance) // 윈도우 클래스 등록
{
    mHInstance = hInstance;
    WNDCLASSEX wcex; // 윈도우클래스 특성관련 구조체
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; //윈도우프로시저 각종이벤트->메세지큐 저장
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));// (COLOR_WINDOW + 1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"윈도우";
    WORD ret = RegisterClassExW(&wcex); // 구조체 전달하여 윈도우 클래스 등록
    return true;
}

bool Window::SetWindow(const WCHAR* szTitle, DWORD dwWindowWidth, DWORD dwWindowHeight) // 윈도우 설정한뒤 윈도우 띄움
{
    mDwWindowWidth = dwWindowWidth;
    mDwWindowHeight = dwWindowHeight;
    mDwWindowPosX = 1920 / 2 - dwWindowWidth / 2;
    mDwWindowPosY = 1080 / 2 - dwWindowHeight / 2;
#ifndef _DEBUG
    dwExStyle = WS_EX_TOPMOST;
    dwStyle = WS_POPUPWINDOW;
#else
    mDwExStyle = WS_EX_APPWINDOW;
#endif
    mHWnd = CreateWindowEx(mDwExStyle, L"윈도우", szTitle, mDwStyle,
        mDwWindowPosX, mDwWindowPosY, mDwWindowWidth, mDwWindowHeight,
        nullptr, nullptr, mHInstance, nullptr);

    if (!mHWnd)
    {
        return FALSE;
    }

    ShowWindow(mHWnd, SW_SHOWNORMAL);
    return true;
}