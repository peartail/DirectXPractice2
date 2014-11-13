#pragma once

#include "TextureShaderClass.h"

class WaterShaderClass : public TextureShaderClass
{
private:
	struct ReflectionBufferType
	{
		D3DXMATRIX reflection;
	};

	struct WaterBufferType
	{
		float waterTranslation;
		float reflectRefractScale;
		D3DXVECTOR2 padding;
	};
public:
	WaterShaderClass();
	~WaterShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,ID3D11ShaderResourceView*,ID3D11ShaderResourceView*,ID3D11ShaderResourceView*,float, float);
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, float);

	virtual void ShutdownShader();

private:
	ID3D11Buffer* _reflectionBuffer;
	ID3D11Buffer* _waterBuffer;

};

