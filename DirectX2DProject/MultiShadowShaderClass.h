#pragma once

#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;



class MultiShadowShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX proj;
		D3DXMATRIX lightView;
		D3DXMATRIX lightProj;
		D3DXMATRIX lightView2;
		D3DXMATRIX lightPorj2;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffueseColor;
		D3DXVECTOR4 diffueseColor2;
	};

	struct LightBufferType2
	{
		D3DXVECTOR3 lightPos;
		float padding;
		D3DXVECTOR3 lightPos2;
		float padding2;
	};
public:
	MultiShadowShaderClass();
	~MultiShadowShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4,D3DXMATRIX,D3DXMATRIX,ID3D11ShaderResourceView*,D3DXVECTOR3,D3DXVECTOR4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleStateWrap;

	ID3D11SamplerState* _sampleStateClamp;

	ID3D11Buffer* _matrixBuffer;
	ID3D11Buffer* _lightBuffer;
	ID3D11Buffer* _lightBuffer2;
};

