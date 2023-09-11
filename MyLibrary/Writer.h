#pragma once
#include "Std.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

struct TextData
{
	wstring text;
	D2D1_RECT_F layout;
	D2D1::ColorF color = { 0.0f, 0.0f, 0.0f, 1.0f };
};

class Writer
{
private:
	Writer() {};
public:
	vector<TextData> mTextList;
	virtual ~Writer() {};

	static Writer& GetInstance()
	{
		static Writer instance;
		return instance;
	}

	ID2D1Factory* mD2DFactory = nullptr;
	IDWriteFactory* mWriteFactory = nullptr;
	IDWriteTextFormat* mTextFormat = nullptr;
	ID2D1RenderTarget* mRT = nullptr;
	ID2D1SolidColorBrush* mBrush = nullptr;
public:
	bool Create(IDXGISurface1* backBuffer);
	bool Init();
	bool Frame();
	bool PreRender();
	bool Render();
	bool PostRender();
	bool Release();
	bool CreateDXWriteRT(IDXGISurface1* surface);
	void AddText(wstring text, float x, float y, D2D1::ColorF color);
	void UpdateText(wstring text, int index);
};

