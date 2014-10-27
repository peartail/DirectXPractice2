#include "FontClass.h"


FontClass::FontClass()
{
}

FontClass::FontClass(const FontClass& other)
{

}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* texutreFilename)
{
	bool result;

	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, texutreFilename);
	if (!result)
	{
		return false;
	}
	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();

}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;

	_font = new FontType[95];
	if (!_font)
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> _font[i].left;
		fin >> _font[i].right;
		fin >> _font[i].size;
	}

	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (_font)
	{
		delete[] _font;
		_font = NULL;
	}

}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	_texture = new TextureClass;
	if (!_texture)
	{
		return false;
	}

	result = _texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = NULL;
	}
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return _texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numletters, index, i, letter;

	vertexPtr = (VertexType*)vertices;

	numletters = (int)strlen(sentence);

	index = 0;

	for (i = 0; i < numletters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{

			float ocha = 1.7123737052044622664189985232392;

			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);	//top left
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left*ocha, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + _font[letter].size), (drawY - 16), 0.0f);	//bottom right
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right*ocha, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f); //bottom left
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left*ocha, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f); // top left
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left*ocha, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + _font[letter].size, drawY, 0.0f); // top right
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right*ocha, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + _font[letter].size), (drawY - 16), 0.0f);	 //bot right
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right*ocha, 1.0f);
			index++;

			drawX = drawX + _font[letter].size + 1.0f;
		}
	}
}







