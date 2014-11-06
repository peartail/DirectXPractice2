#include "TextureArray.h"


TextureArray::TextureArray()
{
	count = 0;
}

TextureArray::TextureArray(const TextureArray&)
{
}

TextureArray::~TextureArray()
{
}


bool TextureArray::Initialize(ID3D11Device* device, WCHAR* file1, WCHAR* file2)
{
	count = 2;
	_textures = new ID3D11ShaderResourceView*[count];

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

bool TextureArray::Initialize(ID3D11Device* device, WCHAR* file1, WCHAR* file2, WCHAR* file3)
{
	count = 3;
	_textures = new ID3D11ShaderResourceView*[count];

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

	result = D3DX11CreateShaderResourceViewFromFile(device, file3, NULL, NULL, &_textures[2], NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureArray::Shutdown()
{
	for (int i = 0; i < count; i++)
	{
		_textures[i]->Release();
		_textures[i] = NULL;
	}
	
}

ID3D11ShaderResourceView** TextureArray::GetTextureArray()
{
	return _textures;
}
