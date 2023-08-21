#include "sample.h"

//문제점: 똑같은 리소스를 계속 반복해서 불러오기 때문에 비효율적 -> 매니저를 만들어서 해소
bool sample::Init()
{
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = false;
    
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_SRC_COLOR 
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; //D3D11_BLEND_SRC_ALPHA
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; //D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    mDevice->CreateBlendState(&blendDesc, &mAlphaBlend);
    mTexMg.Set(mDevice, mImmediateContext);
    mShaMg.Set(mDevice, mImmediateContext);

    srand(time(NULL)); // 현재 시간으로 시드 설정
    wstring texname = L"bg.jpg";
    obj = new PlaneObject;
    obj->Set(mDevice, mImmediateContext);
    obj->SetScale(Vector3(800.0f, 600.0f, 1.0f));
    obj->Create(mTexMg, texname, mShaMg, L"Plane.hlsl");

    for (int i = 0; i < 10; ++i)
    {
        Object* tempObj = new NPC; // 자식으로 캐스팅
        tempObj->SetPos(Vector3((float)randstep(-800.0f, 800.0f), (float)randstep(-800.0f, 800.0f), 0));
        tempObj->Set(mDevice, mImmediateContext);
        tempObj->SetScale(Vector3(50.0f, 50.0f, 1.0f));
        tempObj->Create(mTexMg, L"anajuyo_alpha.png", mShaMg, L"Plane.hlsl");
    }

    return true;
}

bool sample::Frame()
{
    DWORD dwKeyState[256] = { 0, };
    for (int ikey = 0; ikey < 256; ikey++)
    {
        SHORT s = GetAsyncKeyState(ikey);
        if (s & 0x8000) // 1000 0000 0000 0000
        {
            dwKeyState[ikey] = 1;
        }
    }

    if (dwKeyState['A'] == 1)
    {
        mCameraPos.mX -= (float)500.0f * mGameTimer.mSecondPerFrame;
    }
    if (dwKeyState['D'] == 1)
    {
        mCameraPos.mX += (float)500.0f * mGameTimer.mSecondPerFrame;
    }
    if (dwKeyState['W'] == 1)
    {
        mCameraPos.mY += (float)500.0f * mGameTimer.mSecondPerFrame;
    }
    if (dwKeyState['S'] == 1)
    {
        mCameraPos.mY -= (float)500.0f * mGameTimer.mSecondPerFrame;
    }

    obj->Frame();

    for (auto o : mNPCs)
    {
        o->Move(mGameTimer.mSecondPerFrame);
        o->Frame();
    }

    return true;
}

bool sample::Render()
{
    mImmediateContext->OMSetBlendState(mAlphaBlend, 0, -1);
    mMatView._41 = -mCameraPos.mX;
    mMatView._42 = -mCameraPos.mY;
    mMatView._43 = -mCameraPos.mZ;
    mMatOrthonormalProjection._11 = 2.0f / ((float)mDwWindowWidth);
    mMatOrthonormalProjection._22 = 2.0f / ((float)mDwWindowHeight);
    obj->SetMatrix(nullptr, &mMatView, &mMatOrthonormalProjection);
    obj->Render();

    for (auto o : mNPCs)
    {
        o->SetMatrix(nullptr, &mMatView, &mMatOrthonormalProjection);
        o->Render();
    }

    return true;
}
//커맛
bool sample::Release()
{
    obj->Release();
    delete obj;

    for (auto o : mNPCs)
    {
        o->Release();
        delete o;
    }

    mAlphaBlend->Release();
    return true;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) // 메인
{
    sample mySample;
    mySample.SetRegisterWindow(hInstance);
    mySample.SetWindow(L"아무거나", 800, 600);
    mySample.Run();
    return 0;
}