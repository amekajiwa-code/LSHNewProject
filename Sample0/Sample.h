#pragma once
#include "Core.h"
#include "Map.h"
#include "DebugCamera.h"
#include "FBX_Import.h"
#pragma comment(lib, "MyLibrary.lib")

class Model : public Object
{
public:
    FbxMeshStruct* mMeshStruct;
    vector<const Texture*> mTexList;

    void LoadTextureList(wstring fileName)
    {
        mTexList.push_back(TextureManager::GetInstance().Load(fileName));
    }

    bool PostRender()
    {
        for (int i = 0; i < mMeshStruct->triangleList.size(); ++i)
        {
            if (mTexList[i])
            {
                mTexList[i]->Apply(m_pImmediateContext, 0);
            }
            if (m_pIndexBuffer == nullptr)
            {
                m_pImmediateContext->Draw(mMeshStruct->triangleList[i].size(), mMeshStruct->triangleOffsetList[i]);
            }
        }

        return true;
    }
};

class sample : public Core
{
private:
    shared_ptr<Model> mBoxObj;
    shared_ptr<FBX_Import> mFbxImport;
    const Texture* mTex;

    ID3D11BlendState* mAlphaBlend = nullptr;
    shared_ptr<Map> mMapObj;
    shared_ptr<DebugCamera> mDebugCamera = make_shared<DebugCamera>();
public:
    bool  Init();
    bool  Frame();
    bool  Render();
    bool  Release();

    sample() {};
    virtual ~sample() {};
};

