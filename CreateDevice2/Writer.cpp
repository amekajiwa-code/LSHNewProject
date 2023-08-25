#include "Writer.h"

bool Writer::Create(IDXGISurface1* backBuffer)
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &mD2DFactory);

    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            (IUnknown**)&mWriteFactory);
        if (SUCCEEDED(hr))
        {
            mWriteFactory->CreateTextFormat(
                L"고딕",
                NULL,
                DWRITE_FONT_WEIGHT_NORMAL,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                30,
                L"ko-kr", // en-us
                &mTextFormat);
        }
    }

    if (backBuffer)
    {
        if (CreateDXWriteRT(backBuffer))
        {
                return true;
        }
    }

	return false;
}

bool Writer::Init()
{
    

    return true;
}

bool Writer::Frame()
{
	return true;
}

bool Writer::PreRender()
{
    if (mRT)
    {
        mRT->BeginDraw();
        mRT->SetTransform(D2D1::IdentityMatrix());
    }

	return true;
}

bool Writer::Render()
{
    if (PreRender())
    {
        mTextFormat->GetFontSize();
        for (int i = 0; i < mTextList.size(); ++i)
        {
            wstring text = mTextList[i].text;
            D2D1_RECT_F layout = mTextList[i].layout;
            mBrush->SetColor(mTextList[i].color);
            mBrush->SetOpacity(1.0f);
            mRT->DrawText(text.c_str(), text.size(), mTextFormat, &layout, mBrush);
        }

        PostRender();
    }

	return true;
}

bool Writer::PostRender()
{
    if (mRT)
    {
        mRT->EndDraw();
    }

	return true;
}

bool Writer::Release()
{
    if (mD2DFactory) mD2DFactory->Release();
    if (mWriteFactory) mWriteFactory->Release();
    if (mTextFormat) mTextFormat->Release();
    if (mRT) mRT->Release();
    if (mBrush) mBrush->Release();
	return true;
}

bool Writer::CreateDXWriteRT(IDXGISurface1* surface)
{
    //FLOAT xDpi, yDpi;
    //mD2DFactory->GetDesktopDpi(&xDpi, &yDpi);
    float dpi = GetDpiForWindow(g_hWnd);

    D2D1_RENDER_TARGET_PROPERTIES prop = {};
    prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    prop.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    prop.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED; // 미리 계산된 알파값
    prop.dpiX = dpi;
    prop.dpiY = dpi;
    prop.usage = D2D1_RENDER_TARGET_USAGE_NONE;
    prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;


    HRESULT hr = mD2DFactory->CreateDxgiSurfaceRenderTarget(
        surface,
        &prop,
        &mRT
    );

    if (FAILED(hr))
    {
        return false;
    }

    hr = mRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Red),
        &mBrush
    );

    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

void Writer::AddText(wstring text, float x, float y, D2D1::ColorF color)
{
    TextData texts;
    texts.text = text;
    texts.layout = { x, y, static_cast<float>(g_dwWindowWidth), static_cast<float>(g_dwWindowHeight) };
    texts.color = color;

    mTextList.push_back(texts);
}

void Writer::UpdateText(wstring text, int index)
{
    mTextList[index].text = text;
}