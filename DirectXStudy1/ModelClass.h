#pragma once

#ifndef __MODELCLASS_H_
#define __MODELCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>



#define __CHAPTER_SIX__
//#define __CHAPTER_FIVE__
//#define __CHATER_FOUR__

#include "TextureClass.h"


class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

	struct TexVertexType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};

	struct LightVertexType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
		D3DXVECTOR3 nom;
	};
	ID3D11Buffer *m_vertexBuffer,*m_indexBuffer;
	int m_vertexCount,m_indexCount;

	TextureClass* _texture;
public:
	ModelClass(void);
	ModelClass(const ModelClass&);
	~ModelClass(void);

	bool Initailize(ID3D11Device*,WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
};

#endif