#include "TextureShaderClass.h"


TextureShaderClass::TextureShaderClass()
{
	_vertexShader = NULL;
	_pixelShader = NULL;
	_layout = NULL;
	_matrixBuffer = NULL;

	_sampleState = NULL;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{

}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"texture.fx", L"texture.fx");
	if (!result)
	{
		return false;
	}
	return true;
}

void TextureShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}