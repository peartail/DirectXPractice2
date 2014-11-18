#include "OrthoWindowClass.h"


OrthoWindowClass::OrthoWindowClass()
{
	_vertexBuffer = NULL;
	_indexBuffer = NULL;
}


OrthoWindowClass::~OrthoWindowClass()
{
}

bool OrthoWindowClass::Initialize(ID3D11Device* device, int winw, int winh)
{
	bool result;

	result = InitializeBuffers(device, winw, winh);
	if (!result)
	{
		return false;
	}

	return true;
}

void OrthoWindowClass::Shutdown()
{
	ShutdownBuffers();
}

void OrthoWindowClass::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);

}

int OrthoWindowClass::GetIndexCount()
{
	return _indexCnt;
}

bool OrthoWindowClass::InitializeBuffers(ID3D11Device* device, int ww, int wh)
{
	float l, r, t, b;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	l = (float)((ww / 2)*-1);

	r = l + (float)ww;

	t = (float)(wh / 2);

	b = t - (float)wh;

	_vertexCnt = 6;
	_indexCnt = _vertexCnt;

	vertices = new VertexType[_vertexCnt];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[_indexCnt];
	if (!indices)
	{
		return false;
	}

	vertices[0].position = D3DXVECTOR3(l, t, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0, 0);

	vertices[1].position = D3DXVECTOR3(r, b, 0.0f);
	vertices[1].texture = D3DXVECTOR2(1, 1);

	vertices[2].position = D3DXVECTOR3(l, b, 0.0f);
	vertices[2].texture = D3DXVECTOR2(0, 1);

	vertices[3].position = D3DXVECTOR3(l, t, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0, 0);

	vertices[4].position = D3DXVECTOR3(r, t, 0);
	vertices[4].texture = D3DXVECTOR2(1, 0);

	vertices[5].position = D3DXVECTOR3(r, b, 0);
	vertices[5].texture = D3DXVECTOR2(1, 1);

	for (i = 0; i < _indexCnt; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* _vertexCnt;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* _indexCnt;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
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

void OrthoWindowClass::ShutdownBuffers()
{
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = NULL;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
	}

}

void OrthoWindowClass::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}