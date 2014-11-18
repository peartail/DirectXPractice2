#pragma once

#include <D3D11.h>
#include "d3dx10math.h"

class RenderTextureClass
{
public:
	RenderTextureClass();
	~RenderTextureClass();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*);
	void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();

	int GetTextureWidth();
	int GetTextureHeight();

private:
	ID3D11Texture2D* _renderTargetTexture;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _shaderResourceView;

	int _texwidth, _texheight;

	ID3D11DepthStencilView* _depthstencilview;
	D3DXMATRIX _ortho;
};

