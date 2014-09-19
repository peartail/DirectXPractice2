#include "ModelClass.h"


ModelClass::ModelClass(void)
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;

	_texture = NULL;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass(void)
{
}

bool ModelClass::Initailize(ID3D11Device* device, WCHAR* texturefilename)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, texturefilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
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

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return _texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	
	
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc,indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData,indexData;
	HRESULT result;
	
#ifdef __CHAPTER_FOUR__
	m_vertexCount = 8;
	m_indexCount = 36;

	VertexType* vertice;
	//배열 생성
	vertice = new VertexType[m_vertexCount];
	if(!vertice)
	{
		return false;
	}
#elif defined __CHAPTER_FIVE__
	m_vertexCount = 8;
	m_indexCount = 36;

	TexVertexType* vertice;
	//배열 생성
	vertice = new TexVertexType[m_vertexCount];
	if (!vertice)
	{
		return false;
	}
#elif defined __CHAPTER_SIX__
	m_vertexCount = 24;
	m_indexCount = 36;

	LightVertexType* vertice;
	//배열 생성
	vertice = new LightVertexType[m_vertexCount];
	if (!vertice)
	{
		return false;
	}
#endif

	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

#ifdef __CHAPTER_FIVE__
	//정점 배열 값 저장
	vertice[0].pos = D3DXVECTOR3(-1.0f,1.0f,-1.0f);
	vertice[0].tex = D3DXVECTOR2(0.0f,0.0f);

	vertice[1].pos = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	vertice[1].tex = D3DXVECTOR2(0.0f, 1.0f);

	vertice[2].pos = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertice[2].tex = D3DXVECTOR2(1.0f, 0.0f);

	vertice[3].pos = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vertice[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vertice[4].pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertice[4].tex = D3DXVECTOR2(0.0f, 0.0f);

	vertice[5].pos = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertice[5].tex = D3DXVECTOR2(1.0f, 0.0f);

	vertice[6].pos = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertice[6].tex = D3DXVECTOR2(0.0f, 1.0f);

	vertice[7].pos = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertice[7].tex = D3DXVECTOR2(1.0f, 1.0f);

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
#elif defined __CHAPTER_FOUR__
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
#elif defined __CHAPTER_SIX__
	vertice[0].pos = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	vertice[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertice[0].nom = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	vertice[1].pos = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	vertice[1].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertice[1].nom = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	vertice[2].pos = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertice[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertice[2].nom = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	vertice[3].pos = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vertice[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	vertice[3].nom = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//2
	vertice[4].pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertice[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertice[4].nom = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	vertice[5].pos = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertice[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertice[5].nom = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	vertice[6].pos = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertice[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertice[6].nom = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	vertice[7].pos = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertice[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	vertice[7].nom = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//3
	vertice[8].pos = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertice[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertice[8].nom = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	vertice[9].pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertice[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertice[9].nom = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	vertice[10].pos = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	vertice[10].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertice[10].nom = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	vertice[11].pos = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vertice[11].tex = D3DXVECTOR2(1.0f, 1.0f);
	vertice[11].nom = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	//4
	vertice[12].pos = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertice[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertice[12].nom = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertice[13].pos = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertice[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertice[13].nom = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertice[14].pos = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	vertice[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertice[14].nom = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	vertice[15].pos = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertice[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	vertice[15].nom = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	//5
	vertice[16].pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertice[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertice[16].nom = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertice[17].pos = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	vertice[17].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertice[17].nom = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertice[18].pos = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	vertice[18].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertice[18].nom = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	vertice[19].pos = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	vertice[19].tex = D3DXVECTOR2(1.0f, 1.0f);
	vertice[19].nom = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//
	vertice[20].pos = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertice[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertice[20].nom = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vertice[21].pos = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertice[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertice[21].nom = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vertice[22].pos = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertice[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertice[22].nom = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	vertice[23].pos = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vertice[23].tex = D3DXVECTOR2(1.0f, 1.0f);
	vertice[23].nom = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	
	indices[0] = 3;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	indices[6] = 6;
	indices[7] = 4;
	indices[8] = 5;
	indices[9] = 7;
	indices[10] = 4;
	indices[11] = 6;

	indices[12] = 11;
	indices[13] = 9;
	indices[14] = 8;
	indices[15] = 10;
	indices[16] = 9;
	indices[17] = 11;

	indices[18] = 14;
	indices[19] = 12;
	indices[20] = 13;
	indices[21] = 15;
	indices[22] = 12;
	indices[23] = 14;

	indices[24] = 19;
	indices[25] = 17;
	indices[26] = 16;
	indices[27] = 18;
	indices[28] = 17;
	indices[29] = 19;

	indices[30] = 22;
	indices[31] = 20;
	indices[32] = 21;
	indices[33] = 23;
	indices[34] = 20;
	indices[35] = 22;
#endif

	

#ifdef __CHAPTER_FOUR__
	//정점 버퍼의 DESC 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
#elif defined __CHAPTER_FIVE__
	//정점 버퍼의 DESC 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(TexVertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
#elif defined __CHAPTER_SIX__
	//정점 버퍼의 DESC 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(LightVertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
#endif
	

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

#ifdef __CHAPTER_FOUR__
	stride = sizeof(VertexType);
#elif defined __CHAPTER_FIVE__
	stride = sizeof(TexVertexType);
#endif
	
	offset = 0;

	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
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

void ModelClass::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = NULL;
	}
}








