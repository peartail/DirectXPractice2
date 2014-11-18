#pragma once

#include "TextureShaderClass.h"

class VerticalBlurShaderClass : public TextureShaderClass
{
private:
	struct ScreenSizeBufferType
	{
		float screenHeight;
		D3DXVECTOR3 padding;
	};

public:
	VerticalBlurShaderClass();
	~VerticalBlurShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float);
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float);

	virtual void ShutdownShader();

private:
	ID3D11Buffer* _screenSizeBuffer;

};

