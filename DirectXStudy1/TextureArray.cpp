#include "TextureArray.h"


TextureArray::TextureArray()
{
	_textures[0] = NULL;
	_textures[1] = NULL;
}

TextureArray::TextureArray(const TextureArray&)
{
}

TextureArray::~TextureArray()
{
}


bool TextureArray::Initialize(ID3D11Device* device, WCHAR* file1, WCHAR* file2)
{
	bool result;

	result = D3DX11CreateShaderResourceViewFromFile(device, file1, NULL, NULL, &_textures[0], NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFile(device, file2, NULL, NULL, &_textures[1], NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void TextureArray::Shutdown()
{
	if (_textures[0])
	{
		_textures[0]->Release();
		_textures[0] = NULL;
	}

	if (_textures[1])
	{
		_textures[1]->Release();
		_textures[1] = NULL;
	}
}

ID3D11ShaderResourceView** TextureArray::GetTextureArray()
{
	return _textures;
}
