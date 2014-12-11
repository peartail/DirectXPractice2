#include "ParticleSystemClass.h"


ParticleSystemClass::ParticleSystemClass()
{
	_texture = NULL;
	_particleList = NULL;
	_vertices = NULL;
	_vertexBuffer = NULL;
	_indexBuffer = NULL;
}


ParticleSystemClass::~ParticleSystemClass()
{
}



bool ParticleSystemClass::Initialize(ID3D11Device* device, WCHAR* texturefilename)
{
	bool result;

	result = LoadTexture(device, texturefilename);
	if (!result)
	{
		return false;
	}

	result = InitializeParticleSystem();
	if (!result)
	{
		return false;
	}

	result = InitializeBuffer(device);
	if (!result)
	{
		return false;
	}

	return true;
}


void ParticleSystemClass::Shutdown()
{
	ShutdownBuffers();
	ShutdownParticleSystem();
	ReleaseTexture();
	
	
}

bool ParticleSystemClass::Frame(float frametime, ID3D11DeviceContext* context)
{
	bool result;

	KillParticles();

	EmitParticles(frametime);

	UpdateParticles(frametime);

	result = UpdateBuffers(context);
	if (!result)
	{
		return false;
	}

	return true;
}

void ParticleSystemClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ParticleSystemClass::GetIndexCount()
{
	return _indexCnt;
}

ID3D11ShaderResourceView* ParticleSystemClass::GetTexture()
{
	return _texture->GetTexture();
}

bool ParticleSystemClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
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

void ParticleSystemClass::ReleaseTexture()
{

	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = NULL;
	}
}

bool ParticleSystemClass::InitializeParticleSystem()
{
	int i;

	_particleDeviationX = 0.5f;
	_particleDeviationY = 0.1f;
	_particleDeviationZ = 2.f;

	_particleVelocity = 1.f;
	_particleVelocityVariation = 0.2f;

	_particleSize = 0.2f;

	particlePerSecond = 250;

	_maxParticles = 5000;

	_particleList = new ParticleType[_maxParticles];
	if (!_particleList)
	{
		return false;
	}

	for (i = 0; i < _maxParticles; i++)
	{
		_particleList[i].active = false;
	}

	_currentParticleCount = 0;

	_accumuateTime = 0;

	return true;
}

void ParticleSystemClass::ShutdownParticleSystem()
{
	if (_particleList)
	{
		delete[] _particleList;
		_particleList = NULL;
	}
}





bool ParticleSystemClass::InitializeBuffer(ID3D11Device* device)
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	int i;

	_vertexCnt = _maxParticles * 6;
	_indexCnt = _vertexCnt;

	//배열 생성
	_vertices = new VertexType[_vertexCnt];
	if (!_vertices)
	{
		return false;
	}

	indices = new unsigned long[_indexCnt];
	if (!indices)
	{
		return false;
	}

	memset(_vertices, 0, (sizeof(VertexType)*_vertexCnt));

	for (i = 0; i < _indexCnt; ++i)
	{
		indices[i] = i;
	}

	//정점 버퍼의 DESC 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* +_vertexCnt;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = _vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*_indexCnt;
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

	delete[] indices;
	indices = NULL;

	return true;
}

void ParticleSystemClass::ShutdownBuffers()
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

void ParticleSystemClass::EmitParticles(float ftime)
{
	bool emitParticle, found;
	float posX, posY, posZ, velocity, r, g, b;
	int index, i, j;

	_accumuateTime += ftime;

	emitParticle = false;

	if (_accumuateTime > (1000.f / particlePerSecond))
	{
		_accumuateTime = 0;
		emitParticle = true;
	}

	if ((emitParticle == true) && (_currentParticleCount < (_maxParticles - 1)))
	{
		_currentParticleCount++;

		posX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleDeviationX;
		posY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleDeviationY;
		posZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleDeviationZ;

		velocity = _particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX)*_particleVelocityVariation;

		r = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		g = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		b = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

		index = 0;

		found = false;

		while (!found)
		{
			if ((_particleList[index].active == false) || (_particleList[index].posz < posZ))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}

		i = _currentParticleCount;
		j = i - 1;

		while (i != index)
		{
			_particleList[i].posx = _particleList[j].posx;
			_particleList[i].posy = _particleList[j].posy;
			_particleList[i].posz = _particleList[j].posz;
			_particleList[i].red = _particleList[j].red;
			_particleList[i].green = _particleList[j].green;
			_particleList[i].blue = _particleList[j].blue;
			_particleList[i].velocity = _particleList[j].velocity;
			_particleList[i].active = _particleList[j].active;

			i--;
			j--;
		}

		_particleList[index].posx = posX;
		_particleList[index].posy = posY;
		_particleList[index].posz = posZ;
		_particleList[index].red = r;
		_particleList[index].green = g; 
		_particleList[index].blue = b;
		_particleList[index].velocity = velocity;
		_particleList[index].active = true;

	}
}

void ParticleSystemClass::UpdateParticles(float ftime)
{
	int i;

	for (i = 0; i < _currentParticleCount; ++i)
	{
		_particleList[i].posy = _particleList[i].posy - (_particleList[i].velocity * ftime * 0.001f);
	}
}

void ParticleSystemClass::KillParticles()
{
	int i, j;

	for (i = 0; i < _maxParticles; i++)
	{
		if((_particleList[i].active == true) && (_particleList[i].posy < -3.0f))
		{
			_particleList[i].active = false;
			_currentParticleCount--;

			for (j = i; j < _maxParticles - 1; j++)
			{
				_particleList[j].posx = _particleList[j + 1].posx;
				_particleList[j].posy = _particleList[j + 1].posy;
				_particleList[j].posz = _particleList[j + 1].posz;
				_particleList[j].red = _particleList[j + 1].red;
				_particleList[j].green = _particleList[j + 1].green;
				_particleList[j].blue = _particleList[j + 1].blue;
				_particleList[j].velocity = _particleList[j + 1].velocity;
				_particleList[j].active = _particleList[j + 1].active;
			}
		}
	}
}

bool ParticleSystemClass::UpdateBuffers(ID3D11DeviceContext* context)
{
	int index, i;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	memset(_vertices, 0, (sizeof(VertexType)*_vertexCnt));

	index = 0;

	for (i = 0; i < _currentParticleCount; i++)
	{
		_vertices[index].pos = D3DXVECTOR3(_particleList[i].posx - _particleSize, _particleList[i].posy - _particleSize, _particleList[i].posz);
		_vertices[index].tex = D3DXVECTOR2(0, 1);
		_vertices[index].color = D3DXVECTOR4(_particleList[i].red, _particleList[i].green, _particleList[i].blue, 1);
		index++;

		_vertices[index].pos = D3DXVECTOR3(_particleList[i].posx - _particleSize, _particleList[i].posy + _particleSize, _particleList[i].posz);
		_vertices[index].tex = D3DXVECTOR2(0, 0);
		_vertices[index].color = D3DXVECTOR4(_particleList[i].red, _particleList[i].green, _particleList[i].blue, 1);
		index++;

		_vertices[index].pos = D3DXVECTOR3(_particleList[i].posx + _particleSize, _particleList[i].posy - _particleSize, _particleList[i].posz);
		_vertices[index].tex = D3DXVECTOR2(1, 1);
		_vertices[index].color = D3DXVECTOR4(_particleList[i].red, _particleList[i].green, _particleList[i].blue, 1);
		index++;

		_vertices[index].pos = D3DXVECTOR3(_particleList[i].posx + _particleSize, _particleList[i].posy - _particleSize, _particleList[i].posz);
		_vertices[index].tex = D3DXVECTOR2(1, 1);
		_vertices[index].color = D3DXVECTOR4(_particleList[i].red, _particleList[i].green, _particleList[i].blue, 1);
		index++;

		_vertices[index].pos = D3DXVECTOR3(_particleList[i].posx - _particleSize, _particleList[i].posy + _particleSize, _particleList[i].posz);
		_vertices[index].tex = D3DXVECTOR2(0, 0);
		_vertices[index].color = D3DXVECTOR4(_particleList[i].red, _particleList[i].green, _particleList[i].blue, 1);
		index++;

		_vertices[index].pos = D3DXVECTOR3(_particleList[i].posx + _particleSize, _particleList[i].posy + _particleSize, _particleList[i].posz);
		_vertices[index].tex = D3DXVECTOR2(1, 0);
		_vertices[index].color = D3DXVECTOR4(_particleList[i].red, _particleList[i].green, _particleList[i].blue, 1);
		index++;
	}

	result = context->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)_vertices, (sizeof(VertexType)*_vertexCnt));

	context->Unmap(_vertexBuffer, 0);

	return true;
}

void ParticleSystemClass::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);

	offset = 0;

	context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//테셀레이션 쓸 때
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}



