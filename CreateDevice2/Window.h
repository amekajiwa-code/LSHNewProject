#pragma once
#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <ctime>
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib") // d3d11.dll
#pragma comment (lib, "dxgi.lib") // d3d11.dll
#pragma comment (lib, "d3dcompiler.lib") // d3d11.dll
#pragma comment (lib, "DirectXTex.lib") // DirectXTex.h
#include "DirectXTex.h"

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <queue>

#include "GeometryUtils.h"

#define T_PI (3.141592f)
#define T_EPSILON ( 0.0001f)
#define RadianToDegree(radian) ( radian *( 180.0f / T_PI))
#define DegreeToRadian(degree) ( degree *( T_PI / 180.0f))
#define randstep(fmin, fmax) ((float)fmin+((float)fmax-(float)fmin)* rand() / RAND_MAX)

class Window
{
public:
    HINSTANCE   m_hInstance;
    HWND        m_hWnd;
    DWORD       m_dwExStyle = 0;
    DWORD       m_dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD       m_dwWindowPosX = 0;
    DWORD       m_dwWindowPosY = 0;
    DWORD       m_dwWindowWidth;
    DWORD       m_dwWindowHeight;
public:
    bool  SetRegisterClassWindow(HINSTANCE hInstance);
    bool  SetWindow(const WCHAR* szTitle,
        DWORD       dwWindowWidth = 800,
        DWORD       dwWindowHeight = 600);
public:
    virtual ~Window() {}
public:
};