#include "TextClass.h"


TextClass::TextClass()
{
}


TextClass::TextClass(const TextClass& other)
{

}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_baseViewMatrix = baseViewMatrix;

	_font = new FontClass;
	if (!_font)
	{
		return false;
	}

	result = _font->Initialize(device, "fontIndex.txt", L"./Texture/font.gif");
	if (!result)
	{
		MessageBox(hwnd, L"Coud not Init th font obj", L"E", MB_OK);
		return false;
	}


	_fontShader = new FontShaderClass;
	if (!_fontShader)
	{
		return false;
	}

	result = _fontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Coud not init font shader obj", L"E", MB_OK);
		return false;
	}

	result = InitializeSentence(&_sentence1, 16, device);
	if (!result)
	{
		return false;
	}

	result = UpdateSentence(_sentence1, "Hello", 100, 200, 1.0f, 1.0f, 1.0f, context);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&_sentence2, 16, device);
	if (!result)
	{
		return false;
	}

	result = UpdateSentence(_sentence2, "GOODBYE", 100, 20, 1.0f, 1.0f, 0.0f, context);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&_fpsentence, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&_cpusentence, 16, device);
	if (!result)
	{
		return false;
	}

	result = InitializeSentence(&_renderCount, 16, device);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	ReleaseSentence(&_renderCount);

	ReleaseSentence(&_cpusentence);

	ReleaseSentence(&_fpsentence);

	ReleaseSentence(&_sentence1);

	ReleaseSentence(&_sentence2);

	if (_fontShader)
	{
		_fontShader->Shutdown();
		delete _fontShader;
		_fontShader = NULL;
	}

	if (_font)
	{
		_font->Shutdown();
		delete _font;
		_font = NULL;
	}

}

bool TextClass::Render(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX ortho)
{
	bool result;

	result = RenderSentence(context, _sentence1, world, ortho);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(context, _sentence2, world, ortho);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(context, _fpsentence, world, ortho);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(context, _cpusentence, world, ortho);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(context, _renderCount, world, ortho);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	(*sentence)->vertexBuffer = NULL;
	(*sentence)->indexBuffer = NULL;

	(*sentence)->maxLength = maxLength;

	(*sentence)->vertexCount = 6 * maxLength;

	(*sentence)->indexCount = (*sentence)->vertexCount;

	vertices = new VertexType[(*sentence)->vertexCount];

	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType)* (*sentence)->vertexCount));

	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*(*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = NULL;

	delete[] indices;
	indices = NULL;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int posx, int posy, float r, float g, float b, ID3D11DeviceContext* context)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	sentence->red = r;
	sentence->green = g;
	sentence->blue = b;

	numLetters = (int)strlen(text);

	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType)*sentence->vertexCount));

	drawX = (float)(((_screenWidth / 2) * -1) + posx);
	drawY = (float)((_screenHeight / 2) - posy);

	_font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	result = context->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType)*sentence->vertexCount));

	context->Unmap(sentence->vertexBuffer, 0);

	delete[] vertices;
	vertices = NULL;

	return true;

}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = NULL;
		}

		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = NULL;
		}

		delete *sentence;
		*sentence = NULL;
	}


}

bool TextClass::RenderSentence(ID3D11DeviceContext* context, SentenceType* sentence, D3DXMATRIX world, D3DXMATRIX ortho)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pcolor;
	bool result;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pcolor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	result = _fontShader->Render(context, sentence->indexCount, world, _baseViewMatrix, ortho, _font->GetTexture(), pcolor);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;

	_itoa_s(mouseX, tempString, 10);

	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	result = UpdateSentence(_sentence1, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	_itoa_s(mouseY, tempString, 10);

	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	result = UpdateSentence(_sentence2, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	
	return true;
}


bool TextClass::SetFps(int fps, ID3D11DeviceContext* context)
{
	char tempString[16];
	char fpsString[16];
	float r, g, b;
	bool result;

	if (fps > 9999)
	{
		fps = 9999;
	}

	_itoa_s(fps, tempString, 10);

	strcpy_s(fpsString, "Fps : ");
	strcat_s(fpsString, tempString);

	if (fps >= 60)
	{
		r = 0.0f;
		g = 1.0f;
		b = 0.0f;
	}

	if (fps < 60)
	{
		r = 1.0f;
		g = 1.0f;
		b = 0.0f;
	}

	if (fps < 60)
	{
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	result = UpdateSentence(_fpsentence, fpsString, 100, 20, r, g, b, context);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* context)
{
	char tempString[16];
	char cpustring[16];
	bool result;

	_itoa_s(cpu, tempString, 10);

	strcpy_s(cpustring, "Cpu : ");
	strcat_s(cpustring, tempString);
	strcat_s(cpustring, "%");

	result = UpdateSentence(_cpusentence, cpustring, 100, 40, 0.0f, 1.f, 0.f, context);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetRenderCount(int cnt,ID3D11DeviceContext* context)
{
	char tempString[16];
	char cpustring[16];
	bool result;

	_itoa_s(cnt, tempString, 10);

	strcpy_s(cpustring, "Count : ");
	strcat_s(cpustring, tempString);
	strcat_s(cpustring, "%");

	result = UpdateSentence(_renderCount, cpustring, 100, 80, 0.0f, 1.f, 0.f, context);
	if (!result)
	{
		return false;
	}

	return true;

}