#pragma once

#include "TextureShaderClass.h"

class InstanceTextureShaderClass : public TextureShaderClass
{
private:
public:
	InstanceTextureShaderClass();
	~InstanceTextureShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext*, int,int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);

	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	virtual void ShutdownShader();
	virtual void RenderShader(ID3D11DeviceContext*, int,int);
};

