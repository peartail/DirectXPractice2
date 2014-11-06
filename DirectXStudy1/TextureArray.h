#pragma once

#ifndef __TEXTUREARRAYCLASS_H__
#define __TEXTUREARRAYCLASS_H__

#include <D3D11.h>
#include <D3DX11tex.h>


class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();

	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* _textures[2];
};

#endif