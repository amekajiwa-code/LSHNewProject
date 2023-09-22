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

#include <DirectXMath.h>
using namespace DirectX;

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <queue>
#include <codecvt>

#include <wrl/client.h>
using namespace Microsoft::WRL;

#define randstep(fmin, fmax) ((float)fmin+((float)fmax-(float)fmin)* rand() / RAND_MAX)

extern HWND g_hWnd;
extern DWORD g_dwWindowWidth;
extern DWORD g_dwWindowHeight;
extern float g_SecondPerFrame;
extern float g_GameTimer;

using namespace std;

typedef std::basic_string<TCHAR>			T_STR;
typedef std::basic_string<wchar_t>			W_STR;
typedef std::basic_string<char>				C_STR;
typedef std::vector<T_STR>					T_STR_VECTOR;
typedef std::basic_string<TCHAR>::iterator		T_ITOR;
typedef std::basic_string<wchar_t>::iterator	W_ITOR;
typedef std::basic_string<char>::iterator	C_ITOR;
typedef std::vector<T_STR>				T_ARRAY_ITOR;
typedef std::vector<DWORD>				DWORD_VECTOR;
typedef	std::vector< DWORD >::iterator	DWORD_VECTOR_ITOR;
typedef std::list<DWORD>				DWORD_LIST;
typedef std::list<DWORD>::iterator		DWORD_LIST_ITOR;
typedef std::list< HANDLE >				HANDLE_LIST;
typedef	std::list< HANDLE >::iterator	HANDLE_LIST_ITOR;