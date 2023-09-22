#pragma once
#include "Std.h"
#include "Object.h"
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

//FBX�� namespace�� ����. ::

struct FbxMeshStruct
{
	UINT polygonNum = 0;
	//vector<PNCT_Vertex> faceList;
	vector<vector<PNCT_Vertex>> triangleList;
	vector<UINT> triangleOffsetList;
	vector<wstring> textureFileName;
};

class FBX_Import
{
public:
	FbxManager* mFbxManager = nullptr; // �޸� �Ŵ���
	FbxImporter* mFbxImporter = nullptr; // fbx ���� ����Ʈ
	FbxScene* mFbxScene = nullptr; // ȭ�� ������ ������Ʈ��

	FbxNode* mFbxRootNode = nullptr;
	vector<FbxNode*> mFbxNodeList;

	vector<FbxMeshStruct> mFbxMeshList;
public:
	bool Load(wstring fileName);
	bool LoadMesh(FbxNode* node, FbxMeshStruct& fbxTri);
	void PreProcess(FbxNode* fbxNode);
	FbxVector2 ReadTextureCoord(FbxLayerElementUV* uvLayer, int vertexIndex, int uvIndex);
	FbxColor ReadColor(FbxLayerElementVertexColor* layer, int vertexIndex, int index);
	FbxVector4 ReadNormal(FbxLayerElementNormal* layer, int vertexIndex, int index);
	string ParseMaterial(FbxSurfaceMaterial* material);
	int GetSubMaterialIndex(UINT polygon, FbxLayerElementMaterial* material);


	bool Init();
	bool Release();
};

