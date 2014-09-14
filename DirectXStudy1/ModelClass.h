#pragma once

#ifndef __MODELCLASS_H_
#define __MODELCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
	ID3D11Buffer *m_vertexBuffer,*m_indexBuffer;
	int m_vertexCount,m_indexCount;
public:
	ModelClass(void);
	ModelClass(const ModelClass&);
	~ModelClass(void);
	bool Initailize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
};

#endif