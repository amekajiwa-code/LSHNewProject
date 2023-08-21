#pragma once
#include "Window.h"
#include "TextureManager.h"
#include "ShaderManager.h"

struct P3VERTEX {
	Vector3 p; // xyz
	Vector2 t; // uv
};

struct MatrixData
{
	Matrix matWorld; // Scale
	Matrix matView; // Rotation
	Matrix matProjection; // Translate
};

class Object {
public:
	Matrix mMatWorld;
	Matrix mMatView;
	Matrix mMatProjection;
	MatrixData mMatData;

	Vector3 mPos;
	Vector3 mScale;
	Vector3 mRotation;

	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mImmediateContext = nullptr;
public:
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mConstantBuffer = nullptr;
	ID3D11InputLayout* mVertexLayout = nullptr;
	const Shader* mShader = nullptr;
	const Texture* mTexture = nullptr;

	vector<P3VERTEX> verticles;

	void Set(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	bool Create(TextureManager& texMg, wstring texFileName, ShaderManager& shaMg, wstring shaFileName);

	void SetPos(Vector3 pos);
	void SetScale(Vector3 scale);
	void SetMatrix(Matrix* matWorld, Matrix* matView, Matrix* matProjection);

	virtual void Move(float second) {};

	virtual bool CreateVertexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool InputLayout();

	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	Object() {};
	~Object() {};
};