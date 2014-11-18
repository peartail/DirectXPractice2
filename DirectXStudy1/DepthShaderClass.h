#pragma once

#include "TextureShaderClass.h"


class DepthShaderClass : TextureShaderClass
{
public:
	DepthShaderClass();
	~DepthShaderClass();

		virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	virtual void ShutdownShader();
	virtual void RenderShader(ID3D11DeviceContext*, int);

};

