#pragma once


#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;


class RefractionShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX proj;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuse;
		D3DXVECTOR3 lightdir;
		float padding;
	};

	struct ClipPlaneBufferType
	{
		D3DXVECTOR4 clipPlane;
	};
public:
	RefractionShaderClass();
	~RefractionShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4);

	void RotationYawPitchRoll(float, float, float);
	void TranslationMatrix(float, float, float);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, D3DXVECTOR4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;
	ID3D11Buffer* _matrixBuffer;

	ID3D11Buffer* _lightBuffer;

	ID3D11Buffer* _clipPlaneBuffer;

	//내맘대로 변수들
	//회전 한번 해보게
	float yaw, pitch, roll;
	float posx, posy, posz;
};

