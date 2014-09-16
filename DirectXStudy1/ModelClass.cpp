#include "ModelClass.h"


ModelClass::ModelClass(void)
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass(void)
{
}

bool ModelClass::Initailize(ID3D11Device* device)
{
	bool result;
	
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}
	return true;
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertice;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc,indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData,indexData;
	HRESULT result;

	m_vertexCount = 8;
	m_indexCount = 36;

	//배열 생성
	vertice = new VertexType[m_vertexCount];
	if(!vertice)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	//정점 배열 값 저장
	vertice[0].position = D3DXVECTOR3(-1.0f,1.0f,-1.0f);
	vertice[0].color = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);

	vertice[1].position = D3DXVECTOR3(1.0f,1.0f,-1.0f);
	vertice[1].color = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);

	vertice[2].position = D3DXVECTOR3(1.0f,1.0f,1.0f);
	vertice[2].color = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);

	vertice[3].position = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vertice[3].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertice[4].position = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertice[4].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertice[5].position = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertice[5].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertice[6].position = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertice[6].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertice[7].position = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertice[7].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);


	indices[0] = 3;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	indices[6] = 0;
	indices[7] = 5;
	indices[8] = 4;
	indices[9] = 1;
	indices[10] = 5;
	indices[11] = 0;

	indices[12] = 3;
	indices[13] = 4;
	indices[14] = 7;
	indices[15] = 0;
	indices[16] = 4;
	indices[17] = 3;

	indices[18] = 1;
	indices[19] = 6;
	indices[20] = 5;
	indices[21] = 2;
	indices[22] = 6;
	indices[23] = 1;

	indices[24] = 2;
	indices[25] = 7;
	indices[26] = 6;
	indices[27] = 3;
	indices[28] = 7;
	indices[29] = 2;

	indices[30] = 6;
	indices[31] = 4;
	indices[32] = 5;
	indices[33] = 7;
	indices[34] = 4;
	indices[35] = 6;


	//정점 버퍼의 DESC 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertice;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertice;
	vertice = NULL;

	delete[] indices;
	indices = NULL;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = NULL;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}












