#include "FBX_Import.h"

string wstring_to_string(const wstring& wideStr) {
	string narrowStr;
	narrowStr.assign(wideStr.begin(), wideStr.end());
	return narrowStr;
}

FbxVector2 FBX_Import::ReadTextureCoord(FbxLayerElementUV* uvLayer, int uvIndex)
{
	FbxVector2 uv;
	switch (uvLayer->GetMappingMode())
	{
		// ������ �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //�迭���� ���� ��´�.
		{

		}break;
		case FbxLayerElementUV::eIndexToDirect:// �ε����� ���ؼ� �迭�� �����ؼ� ��´�.
		{

		}break;
		}
	}break;
	// ���� �� 1���� �ؽ�ó ��ǥ�� �ִ�.
	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			uv = uvLayer->GetDirectArray().GetAt(uvIndex);
		}break;
		}
	}break;
	}
	return uv;
}

//Ʈ�� ��ȸ
void FBX_Import::PreProcess(FbxNode* fbxNode)
{
	if (fbxNode == nullptr) return;

	string name = fbxNode->GetName();
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	if (fbxMesh != nullptr)
	{
		mFbxNodeList.push_back(fbxNode);
	}

	UINT childCount = fbxNode->GetChildCount();

	for (int i = 0; i < childCount; ++i)
	{
		FbxNode* childNode = fbxNode->GetChild(i);
		PreProcess(childNode);
	}
}

bool FBX_Import::Load(wstring fileName)
{
	string inputName = wstring_to_string(fileName);
	bool ret = mFbxImporter->Initialize(inputName.c_str());
	ret = mFbxImporter->Import(mFbxScene);

	FbxNode* FbxRootNode = mFbxScene->GetRootNode();
	if (FbxRootNode)
	{
		PreProcess(FbxRootNode);
	}

	for (int i = 0; i < mFbxNodeList.size(); ++i)
	{
		FbxTriangle fbxTri;
		LoadMesh(mFbxNodeList[i], fbxTri);
		mTriList.push_back(fbxTri);
	}

	return true;
}

bool FBX_Import::LoadMesh(FbxNode* node, FbxTriangle& fbxTri)
{
	FbxMesh* fbxMesh = node->GetMesh();
	UINT polygonCount = fbxMesh->GetPolygonCount();
	FbxVector4* vertexPos = fbxMesh->GetControlPoints();

	UINT layerCount = fbxMesh->GetLayerCount();
	vector<FbxLayerElementUV*> vertexUVSetList;
	for (int i = 0; i < layerCount; ++i)
	{
		FbxLayer* layer = fbxMesh->GetLayer(i);
		if (layer->GetUVs() != nullptr)
		{
			vertexUVSetList.push_back(layer->GetUVs());
		}
	}

	// 1. �簢���̸� face 2�� ���������� 4�� 2. �ﰢ���̸� face 1�� ���������� 3��
	for (int i = 0; i < polygonCount; ++i)
	{
		UINT polygonSize = fbxMesh->GetPolygonSize(i);
		UINT triangleCount = polygonSize - 2;
		UINT cornerIndices[3];

		for (int j = 0; j < triangleCount; ++j)
		{
			//�ƽ� ��ǥ��� y, z�� �ݴ��̹Ƿ� ���� ��ǥ�� �°� ��Ī
			cornerIndices[0] = fbxMesh->GetPolygonVertex(i, 0);
			cornerIndices[1] = fbxMesh->GetPolygonVertex(i, j + 2);
			cornerIndices[2] = fbxMesh->GetPolygonVertex(i, j + 1);

			int u[3];
			u[0] = fbxMesh->GetTextureUVIndex(i, 0);
			u[1] = fbxMesh->GetTextureUVIndex(i, j + 2);
			u[2] = fbxMesh->GetTextureUVIndex(i, j + 1);

			//�ﰢ�� ������ �ִ� 3��
			for (int k = 0; k < 3; ++k)
			{
				UINT conerIndex = cornerIndices[k];
				PNCT_Vertex pnct;
				FbxVector4 tempVec = vertexPos[conerIndex];
				// y <-> z
				pnct.p.x = tempVec.mData[0];
				pnct.p.y = tempVec.mData[2];
				pnct.p.z = tempVec.mData[1];
				pnct.n = { 0.0f, 0.0f, 0.0f };
				pnct.c = { 1.0f, 1.0f, 1.0f, 1.0f };

				if (vertexUVSetList.size() > 0)
				{
					FbxLayerElementUV* uvLayer = vertexUVSetList[0];
					FbxVector2 uv(0, 0);
					uv = ReadTextureCoord(uvLayer, u[k]);
					pnct.t = { (float)uv.mData[0],(float)(1.0f - uv.mData[1]) };
				}
				fbxTri.faceList.push_back(pnct);
			}
		}
	}

	return true;
}

bool FBX_Import::Init()
{
	mFbxManager = FbxManager::Create();
	mFbxImporter = FbxImporter::Create(mFbxManager, IOSROOT);
	mFbxScene = FbxScene::Create(mFbxManager, "");
	return true;
}

bool FBX_Import::Release()
{
	//������ ������ ����
	if (mFbxScene) mFbxScene->Destroy();
	if (mFbxImporter) mFbxImporter->Destroy();
	if (mFbxManager) mFbxManager->Destroy();

	mFbxScene = nullptr;
	mFbxImporter = nullptr;
	mFbxManager = nullptr;
	return true;
}