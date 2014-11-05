#pragma once

#ifndef __UICLASS_H__
#define __UICLASS_H__

#include <D3D11.h>
#include <D3DX10math.h>

#include "TextureClass.h"

class UIClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	UIClass();
	UIClass(const UIClass&);
	~UIClass();

	bool Initialize(ID3D11Device*, int, int, WCHAR*, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	D3DXMATRIX GetBaseViewMatrix();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	int _vertexCount, _indexCount;
	TextureClass* _texture;

	int _screenWidth, _screenHeight;
	int _bitmapWidth, _bitmapHeight;
	int _previousPosX, _previousPosY;

	D3DXMATRIX _baseViewMatrix;
};

#endif