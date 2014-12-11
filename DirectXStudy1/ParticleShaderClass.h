#pragma once

#include "TextureShaderClass.h"

class ParticleShaderClass : public TextureShaderClass
{
public:
	ParticleShaderClass();
	~ParticleShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

	virtual void ShutdownShader();
	virtual void RenderShader(ID3D11DeviceContext*, int);

private:

};

