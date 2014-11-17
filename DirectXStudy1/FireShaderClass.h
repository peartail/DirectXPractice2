#pragma once

#include "TextureShaderClass.h"

class FireShaderClass : public TextureShaderClass
{
private:
	struct NoiseBufferType
	{
		float frameTime;
		D3DXVECTOR3 scrollSpeed;
		D3DXVECTOR3 scale;
		float padding;
	};

	struct DistortionBufferType
	{
		D3DXVECTOR2 dist1;
		D3DXVECTOR2 dist2;
		D3DXVECTOR2 dist3;
		float distScale;
		float distBias;
	};
public:
	FireShaderClass();
	~FireShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	virtual bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, D3DXVECTOR2, D3DXVECTOR2, float, float);

	virtual void ShutdownShader();
	virtual void RenderShader(ID3D11DeviceContext*, int);
private:
	ID3D11Buffer* _noiseBuffer;
	ID3D11SamplerState* _sampleState2;

	ID3D11Buffer* _distortBuffer;

};

