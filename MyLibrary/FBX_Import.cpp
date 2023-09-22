#include "FBX_Import.h"

string wstring_to_string(const wstring& wideStr) {
	string narrowStr;
	narrowStr.assign(wideStr.begin(), wideStr.end());
	return narrowStr;
}

wstring string_to_wstring(const std::string& narrowStr) {
	std::wstring wideStr;
	wideStr.assign(narrowStr.begin(), narrowStr.end());
	return wideStr;
}

int FBX_Import::GetSubMaterialIndex(UINT polygon, FbxLayerElementMaterial* material)
{
	int subMaterial = 0;

	switch (material->GetMappingMode())
	{
	case FbxLayerElementUV::eByPolygon:
		switch (material->GetReferenceMode())
		{
		case FbxLayerElementUV::eIndex:
			subMaterial = polygon;
			break;
		case FbxLayerElementUV::eIndexToDirect:
			subMaterial = material->GetIndexArray().GetAt(polygon);
			break;
		}
		break;
	}

	return subMaterial;
}

string FBX_Import::ParseMaterial(FbxSurfaceMaterial* material)
{
	string texName;
	auto Property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (Property.IsValid())
	{
		FbxFileTexture* tex = Property.GetSrcObject<FbxFileTexture>(0);
		if (tex != nullptr)
		{
			string filename = tex->GetFileName();
			return filename;
		}
	}
	return texName;
}

FbxVector4 FBX_Import::ReadNormal(FbxLayerElementNormal* layer, int vertexIndex, int index)
{
	FbxVector4 normal;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
			normal = layer->GetDirectArray().GetAt(vertexIndex);
			break;
		case FbxLayerElementUV::eIndexToDirect:
			int id = layer->GetIndexArray().GetAt(vertexIndex);
			normal = layer->GetDirectArray().GetAt(id);
			break;
		}
		break;
	case FbxLayerElementUV::eByPolygonVertex:
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
			normal = layer->GetDirectArray().GetAt(index);
			break;
		case FbxLayerElementUV::eIndexToDirect:
			int id = layer->GetIndexArray().GetAt(index);
			normal = layer->GetDirectArray().GetAt(id);
			break;
		}
		break;
	}

	return normal;
}

FbxColor FBX_Import::ReadColor(FbxLayerElementVertexColor* layer, int vertexIndex, int index)
{
	FbxColor color(1.0f, 1.0f, 1.0f, 1.0f);

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
			color = layer->GetDirectArray().GetAt(vertexIndex);
			break;
		case FbxLayerElementUV::eIndexToDirect:
			int id = layer->GetIndexArray().GetAt(vertexIndex);
			color = layer->GetDirectArray().GetAt(id);
			break;
		}
		break;
	case FbxLayerElementUV::eByPolygonVertex:
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
			color = layer->GetDirectArray().GetAt(index);
			break;
		case FbxLayerElementUV::eIndexToDirect:
			int id = layer->GetIndexArray().GetAt(index);
			color = layer->GetDirectArray().GetAt(id);
			break;
		}
		break;
	}

	return color;
}

FbxVector2 FBX_Import::ReadTextureCoord(FbxLayerElementUV* uvLayer, int vertexIndex, int uvIndex)
{
	FbxVector2 uv;

	switch (uvLayer->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint: // 제어점 당 하나의 매핑 좌표
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
			uv = uvLayer->GetDirectArray().GetAt(vertexIndex);
			break;
		case FbxLayerElementUV::eIndexToDirect:
			int id = uvLayer->GetIndexArray().GetAt(vertexIndex);
			uv = uvLayer->GetDirectArray().GetAt(id);
			break;
		}
		break;
	case FbxLayerElementUV::eByPolygonVertex: // 정점 당 1개의 매핑 좌표
		switch (uvLayer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
			uv = uvLayer->GetDirectArray().GetAt(uvIndex);
			break;
		}
		break;
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
		FbxMeshStruct fbxTri;
		LoadMesh(mFbxNodeList[i], fbxTri);
		mFbxMeshList.push_back(fbxTri);
	}

	return true;
}

bool FBX_Import::LoadMesh(FbxNode* node, FbxMeshStruct& fbxTri)
{
#pragma region 정점매핑
	FbxMesh* fbxMesh = node->GetMesh();
	UINT polygonCount = fbxMesh->GetPolygonCount();
	FbxVector4* vertexPos = fbxMesh->GetControlPoints();

	UINT layerCount = fbxMesh->GetLayerCount();
	vector<FbxLayerElementUV*> vertexUVSetList;
	vector<FbxLayerElementNormal*> vertexNormalSetList;
	vector<FbxLayerElementVertexColor*> vertexColorSetList;
	vector<FbxLayerElementMaterial*> materialSetList;
	for (int i = 0; i < layerCount; ++i)
	{
		FbxLayer* layer = fbxMesh->GetLayer(i);
		if (layer->GetUVs() != nullptr)
		{
			vertexUVSetList.push_back(layer->GetUVs());
		}
		if (layer->GetNormals() != nullptr)
		{
			vertexNormalSetList.push_back(layer->GetNormals());
		}
		if (layer->GetVertexColors() != nullptr)
		{
			vertexColorSetList.push_back(layer->GetVertexColors());
		}
		if (layer->GetMaterials() != nullptr)
		{
			materialSetList.push_back(layer->GetMaterials());
		}
	}
#pragma endregion

#pragma region Material
	int materialNum = node->GetMaterialCount();

	if (materialNum > 1)
	{
		fbxTri.triangleList.resize(materialNum);
	}
	else
	{
		fbxTri.triangleList.resize(1);
	}

	for (int i = 0; i < materialNum; ++i)
	{
		FbxSurfaceMaterial* pSurface = node->GetMaterial(i);
		if (pSurface)
		{
			string texturename = ParseMaterial(pSurface);
			CHAR Drive[MAX_PATH];
			CHAR Dir[MAX_PATH];
			CHAR FName[MAX_PATH];
			CHAR Ext[MAX_PATH];
			_splitpath_s(texturename.c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
			string texName = FName;
			texName += Ext;
			fbxTri.textureFileName.push_back(string_to_wstring(texName));
		}
	}

	FbxAMatrix GeometircMatrix; // pos
	FbxAMatrix normalMatrix; // normal
	{
		FbxVector4 t = node->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 s = node->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxVector4 r = node->GetGeometricRotation(FbxNode::eSourcePivot);

		GeometircMatrix.SetT(t);
		GeometircMatrix.SetR(r);
		GeometircMatrix.SetS(s);

		normalMatrix = GeometircMatrix;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();
	}
#pragma endregion

#pragma region 페이스 DX좌표계로 변환
	// 1. 사각형이면 face 2개 폴리곤정점 4개 2. 삼각형이면 face 1개 폴리곤정점 3개
	UINT  basePolyIndex = 0;
	for (int i = 0; i < polygonCount; ++i)
	{
		// 서브메터리얼 검색
		int subMaterial = 0;
		if (materialNum >= 1)
		{
			subMaterial = GetSubMaterialIndex(i, materialSetList[0]);
		}

		UINT polygonSize = fbxMesh->GetPolygonSize(i);
		UINT triangleCount = polygonSize - 2;
		UINT cornerIndices[3];
		fbxTri.polygonNum += triangleCount;

		for (int j = 0; j < triangleCount; ++j)
		{
			UINT colorIndex[3] = { 0, j + 2, j + 1 };
			//맥스 좌표계는 y, z가 반대이므로 현재 좌표계 맞게 매칭
			cornerIndices[0] = fbxMesh->GetPolygonVertex(i, colorIndex[0]);
			cornerIndices[1] = fbxMesh->GetPolygonVertex(i, colorIndex[1]);
			cornerIndices[2] = fbxMesh->GetPolygonVertex(i, colorIndex[2]);

			int u[3];
			u[0] = fbxMesh->GetTextureUVIndex(i, colorIndex[0]);
			u[1] = fbxMesh->GetTextureUVIndex(i, colorIndex[1]);
			u[2] = fbxMesh->GetTextureUVIndex(i, colorIndex[2]);

			//삼각형 정점은 최대 3개
			for (int k = 0; k < 3; ++k)
			{
				UINT conerIndex = cornerIndices[k];
				PNCT_Vertex pnct;
				FbxVector4 tempVec = vertexPos[conerIndex];

				auto finalPos = GeometircMatrix.MultT(tempVec);

				// y <-> z
				pnct.p.x = tempVec.mData[0];
				pnct.p.y = tempVec.mData[2];
				pnct.p.z = tempVec.mData[1];

				if (vertexUVSetList.size() > 0)
				{
					FbxLayerElementUV* uvLayer = vertexUVSetList[0];
					FbxVector2 uv(0, 0);
					uv = ReadTextureCoord(uvLayer, conerIndex, u[k]);
					pnct.t = { (float)uv.mData[0],(float)(1.0f - uv.mData[1]) };
				}

				pnct.c = { 1.0f, 1.0f, 1.0f, 1.0f };
				if (vertexColorSetList.size() > 0)
				{
					FbxColor color = ReadColor(vertexColorSetList[0], conerIndex, basePolyIndex + colorIndex[k]);

					pnct.c.x = color.mRed;
					pnct.c.y = color.mGreen;
					pnct.c.z = color.mBlue;
					pnct.c.w = 1.0f;
				}

				pnct.n = { 0.0f, 0.0f, 0.0f };
				if (vertexNormalSetList.size() > 0)
				{
					FbxVector4 normal = ReadNormal(vertexNormalSetList[0], conerIndex, basePolyIndex + colorIndex[k]);
					auto finalNormal = normalMatrix.MultT(normal);
					finalNormal.Normalize();

					pnct.n.x = finalNormal.mData[0];
					pnct.n.x = finalNormal.mData[2];
					pnct.n.x = finalNormal.mData[1];
				}

				fbxTri.triangleList[subMaterial].push_back(pnct);
			}
		}

		basePolyIndex += polygonSize;
	}

#pragma endregion

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