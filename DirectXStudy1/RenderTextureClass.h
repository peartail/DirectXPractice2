#pragma once

#include <D3D11.h>
#include "d3dx10math.h"

class RenderTextureClass
{
public:
	RenderTextureClass();
	~RenderTextureClass();

	bool Initialize(ID3D11Device*, int, int);
	bool Initialize(ID3D11Device*, int, int,float,float);
	void Shutdown();

	
	void SetRenderTarget(ID3D11DeviceContext*);
	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*,float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();

	int GetTextureWidth();
	int GetTextureHeight();
	void GetOrthoMatrix(D3DXMATRIX&);
private:
	ID3D11Texture2D* _renderTargetTexture;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _shaderResourceView;

	int _texwidth, _texheight;

	ID3D11DepthStencilView* _depthstencilview;
	ID3D11Texture2D* _depthStencilBuffer;
	D3D11_VIEWPORT _viewport;
	D3DXMATRIX _projectionMatrix;
	D3DXMATRIX _ortho;
};

