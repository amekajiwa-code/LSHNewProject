#pragma once
#include "Std.h"
#include "Object.h"
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

//FBX�� namespace�� ����. ::

struct FbxTriangle
{
	vector<PNCT_Vertex> faceList;
};

class FBX_Import
{
public:
	FbxManager* mFbxManager = nullptr; // �޸� �Ŵ���
	FbxImporter* mFbxImporter = nullptr; // fbx ���� ����Ʈ
	FbxScene* mFbxScene = nullptr; // ȭ�� ������ ������Ʈ��
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

