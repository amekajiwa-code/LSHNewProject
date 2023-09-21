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
		// 제어점 당 1개의 텍스처 좌표가 있다.
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect: //배열에서 직접 얻는다.
		{

		}break;
		case FbxLayerElementUV::eIndexToDirect:// 인덱스를 통해서 배열에 접근해서 얻는다.
		{

		}break;
		}
	}break;
	// 정점 당 1개의 텍스처 좌표가 있다.
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

//트리 순회
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

	// 1. 사각형이면 face 2개 폴리곤정점 4개 2. 삼각형이면 face 1개 폴리곤정점 3개
	for (int i = 0; i < polygonCount; ++i)
	{
		UINT polygonSize = fbxMesh->GetPolygonSize(i);
		UINT triangleCount = polygonSize - 2;
		UINT cornerIndices[3];

		for (int j = 0; j < triangleCount; ++j)
		{
			//맥스 좌표계는 y, z가 반대이므로 현재 좌표계 맞게 매칭
			cornerIndices[0] = fbxMesh->GetPolygonVertex(i, 0);
			cornerIndices[1] = fbxMesh->GetPolygonVertex(i, j + 2);
			cornerIndices[2] = fbxMesh->GetPolygonVertex(i, j + 1);

			int u[3];
			u[0] = fbxMesh->GetTextureUVIndex(i, 0);
			u[1] = fbxMesh->GetTextureUVIndex(i, j + 2);
			u[2] = fbxMesh->GetTextureUVIndex(i, j + 1);

			//삼각형 정점은 최대 3개
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
	//삭제는 생성의 역순
	if (mFbxScene) mFbxScene->Destroy();
	if (mFbxImporter) mFbxImporter->Destroy();
	if (mFbxManager) mFbxManager->Destroy();

	mFbxScene = nullptr;
	mFbxImporter = nullptr;
	mFbxManager = nullptr;
	return true;
}