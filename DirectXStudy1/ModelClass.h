#pragma once

#ifndef __MODELCLASS_H_
#define __MODELCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>

#include <fstream>
using namespace std;

#include "TextureClass.h"

#include "TextureArray.h"


class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binorm;
	};
	struct ModelType
	{
		float x,y,z;
		float tu,tv;
		float nx,ny,nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

	ID3D11Buffer *m_vertexBuffer,*m_indexBuffer;
	int m_vertexCount,m_indexCount;

	TextureClass* _texture;
	ModelType* _model;
	TextureArray* _texarr;
public:
	ModelClass(void);
	ModelClass(bool);
	ModelClass(const ModelClass&);
	~ModelClass(void);

	bool Initailize(ID3D11Device*,char*,WCHAR*);
	bool Initailize(ID3D11Device*, char*, WCHAR*,WCHAR*);
	bool Initailize(ID3D11Device*, char*, WCHAR*, WCHAR*,WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	ID3D11ShaderResourceView** GetTextureArr();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	bool LoadTexture(ID3D11Device*, WCHAR*,WCHAR*);
	bool LoadTexture(ID3D11Device*, WCHAR*, WCHAR*,WCHAR*);
	void ReleaseTexture();
	bool LoadModel(ID3D11Device*, char*);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

private:
	bool _isSdkMesh;
	
};

#endif