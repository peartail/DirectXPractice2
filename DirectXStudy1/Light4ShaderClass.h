#pragma once

const int NUM_LIGHTS = 4;

#include "TextureShaderClass.h"

class Light4ShaderClass : public TextureShaderClass
{
private:
	struct LightColorBufferType
	{
		D3DXVECTOR4 difColor[NUM_LIGHTS];
	};

	struct LightPositionBufferType
	{
		D3DXVECTOR4 lightPos[NUM_LIGHTS];
	};
public:
	Light4ShaderClass();
	~Light4ShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4[],D3DXVECTOR4[]);
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4[],D3DXVECTOR4[]);

	virtual void ShutdownShader();

private:
	ID3D11Buffer* _lightColorBuffer;
	ID3D11Buffer* _lightPosBuffer;
};

