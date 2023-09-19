#include "Window.h"
HWND g_hWnd;
DWORD g_dwWindowWidth;
DWORD g_dwWindowHeight;
Window* g_pWindow = nullptr;

int Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (SIZE_MINIMIZED != wParam) // 최소화가 아니면
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            ResizeDevice(width, height);
        }
        break;
    }
    return -1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT hr = g_pWindow->MsgProc(hWnd, message, wParam, lParam);
    if (hr > 0) return 0;

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool Window::SetRegisterClassWindow(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; //윈도우프로시저 각종이벤트->메세지큐 저장
    wcex.hInstance = hInstance;
    wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));// (COLOR_WINDOW + 1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"윈도우";
    WORD ret = RegisterClassExW(&wcex);

    return true;
}

bool Window::SetWindow(const WCHAR* szTitle, DWORD dwWindowWidth, DWORD dwWindowHeight)
{
    g_dwWindowWidth = m_dwWindowWidth = dwWindowWidth;
    g_dwWindowHeight = m_dwWindowHeight = dwWindowHeight;
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
    m_dwWindowPosX = screenWidth / 2 - dwWindowWidth / 2;
    m_dwWindowPosY = screenHeight / 2 - dwWindowHeight / 2;
    

#ifndef _DEBUG
    m_dwExStyle = WS_EX_TOPMOST;
    m_dwStyle = WS_POPUPWINDOW;
#else
    m_dwExStyle = WS_EX_APPWINDOW;
#endif
    RECT rect = { 0, 0, dwWindowWidth, dwWindowHeight };
    AdjustWindowRect(&rect, m_dwStyle, FALSE);
    m_hWnd = CreateWindowEx(
        m_dwExStyle,
        L"윈도우",
        szTitle,
        m_dwStyle,
        m_dwWindowPosX, m_dwWindowPosY,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr,
        nullptr,
        m_hInstance,
        nullptr);

    if (!m_hWnd)
    {
        return FALSE;
    }
    g_hWnd = m_hWnd;

    GetClientRect(m_hWnd, &mRectClient);
    g_dwWindowWidth = g_dwWindowWidth = mRectClient.right;
    g_dwWindowHeight = g_dwWindowHeight = mRectClient.bottom;

    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    return true;
}

Window::Window()
{
    g_pWindow = this;
}