#pragma once

#include <D3D11.h>
#include <D3DX10math.h>

class DebugWindowsClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 tex;
	};
public:
	DebugWindowsClass();
	~DebugWindowsClass();

	bool Initialize(ID3D11Device*, int, int, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;

	int _vertexCount, _indexCount;
	int _screenWidth, _screenHeight;
	int _bitmapWidth, _bitmapHeight;
	int _previousPosX, _previousPosY;
};

