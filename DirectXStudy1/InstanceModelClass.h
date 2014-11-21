#pragma once

#include <d3d11.h>
#include <D3DX10math.h>

#include <fstream>
using namespace std;

#include "TextureClass.h"


class InstanceModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;

	};
	struct InstanceType
	{
		D3DXVECTOR3 pos;
	};

public:
	InstanceModelClass();
	~InstanceModelClass();

	bool Initailize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InstanceModelClass::LoadModel(ID3D11Device*, char*);
	void ReleaseModel();
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* _vertexBuffer;

	ID3D11Buffer* _instanceBuffer;
	int _vertexCnt;

	int _instanceCnt;
	ModelType* _model;
	TextureClass*_texture;
};

