#pragma once
#define _CRT_SECURE_NO_WARNINGS

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
#include <codecvt>

#include "GeometryUtils.h"

#define randstep(fmin, fmax) ((float)fmin+((float)fmax-(float)fmin)* rand() / RAND_MAX)

extern HWND g_hWnd;
extern DWORD g_dwWindowWidth;
extern DWORD g_dwWindowHeight;
extern float g_SecondPerFrame;
extern float g_GameTimer;

using namespace std;