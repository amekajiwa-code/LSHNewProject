#pragma once
#include "Std.h"
#include "Object.h"
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

//FBX는 namespace가 없다. ::

struct FbxTriangle
{
	vector<PNCT_Vertex> faceList;
};

class FBX_Import
{
public:
	FbxManager* mFbxManager = nullptr; // 메모리 매니저
	FbxImporter* mFbxImporter = nullptr; // fbx 파일 임포트
	FbxScene* mFbxScene = nullptr; // 화면 렌더링 오브젝트들
	vector<FbxNode*> mFbxNodeList;
	vector<FbxTriangle> mTriList;
public:
	bool Load(wstring fileName);
	bool LoadMesh(FbxNode* node, FbxTriangle& fbxTri);
	void PreProcess(FbxNode* fbxNode);
	FbxVector2 ReadTextureCoord(FbxLayerElementUV* uvLayer, int uvIndex);
	bool Init();
	bool Release();
};

