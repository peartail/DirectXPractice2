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

	m_vertexCount = 3;
	m_indexCount = 3;

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

	vertice[0].position = D3DXVECTOR3(-1.0f,-1.0f,0.0f);
	vertice[0].color = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);

	vertice[1].position = D3DXVECTOR3(0.0f,1.0f,0.0f);
	vertice[1].color = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);

	vertice[2].position = D3DXVECTOR3(1.0f,-1.0f,0.0f);
	vertice[2].color = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	
}