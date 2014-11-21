#include "InstanceModelClass.h"


InstanceModelClass::InstanceModelClass()
{
	_vertexBuffer = NULL;

	_texture = NULL;

	_instanceBuffer = NULL;
	_model = NULL;
}


InstanceModelClass::~InstanceModelClass()
{
}



bool InstanceModelClass::Initailize(ID3D11Device* device, char* modelfilename, WCHAR* texturefilename)
{
	bool result;

	result = LoadModel(device, modelfilename);
	if (!result)
	{
		return false;
	}

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

void InstanceModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
}

void InstanceModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}



ID3D11ShaderResourceView* InstanceModelClass::GetTexture()
{
	return _texture->GetTexture();
}

int InstanceModelClass::GetVertexCount()
{
	return _vertexCnt;
}

int InstanceModelClass::GetInstanceCount()
{
	return _instanceCnt;
}

bool InstanceModelClass::InitializeBuffers(ID3D11Device* device)
{

	VertexType* vertice;
	InstanceType* instances;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, instanceData;
	HRESULT result;

	int i;

	//배열 생성
	vertice = new VertexType[_vertexCnt];
	if (!vertice)
	{
		return false;
	}

	for (i = 0; i<_vertexCnt; i++)
	{
		vertice[i].pos = D3DXVECTOR3(_model[i].x, _model[i].y, _model[i].z);
		vertice[i].tex = D3DXVECTOR2(_model[i].tu, _model[i].tv);

		//indices[i] = i;
	}

	//정점 버퍼의 DESC 작성
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* _vertexCnt;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertice;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertice;
	vertice = NULL;

	_instanceCnt = 4;

	instances = new InstanceType[_instanceCnt];
	if (!instances)
	{
		return false;
	}

	instances[0].pos = D3DXVECTOR3(-5, -5, 5);
	instances[1].pos = D3DXVECTOR3(5, -5, 5);
	instances[2].pos = D3DXVECTOR3(-5, 5, 5);
	instances[3].pos = D3DXVECTOR3(5, 5, 5);

	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType)* _instanceCnt;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &_instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] instances;
	instances = NULL;

	return true;
}

void InstanceModelClass::ShutdownBuffers()
{
	if (_instanceBuffer)
	{
		_instanceBuffer->Release();
		_instanceBuffer = NULL;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
	}
}

void InstanceModelClass::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int stride[2];
	unsigned int offset[2];
	ID3D11Buffer* bufferPointers[2];

	//stride = sizeof(VertexType);
	stride[0] = sizeof(VertexType);
	stride[1] = sizeof(InstanceType);

	offset[0] = 0;
	offset[1] = 0;

	bufferPointers[0] = _vertexBuffer;
	bufferPointers[1] = _instanceBuffer;
	//offset = 0;

	context->IASetVertexBuffers(0, 2, bufferPointers, stride, offset);

//	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool InstanceModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
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

void InstanceModelClass::ReleaseTexture()
{

	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = NULL;
	}
}

bool InstanceModelClass::LoadModel(ID3D11Device* device, char* filename)
{
		ifstream fin;
		char input;
		int i;

		fin.open(filename);

		if (fin.fail())
		{
			return false;
		}

		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		fin >> _vertexCnt;

		//m_indexCount = _vertexCnt;

		_model = new ModelType[_vertexCnt];
		if (!_model)
		{
			return false;
		}

		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		for (i = 0; i<_vertexCnt; i++)
		{
			fin >> _model[i].x >> _model[i].y >> _model[i].z;
			fin >> _model[i].tu >> _model[i].tv;
			fin >> _model[i].nx >> _model[i].ny >> _model[i].nz;
		}

		fin.close();


	return true;
}

void InstanceModelClass::ReleaseModel()
{
	if (_model)
	{
		delete[] _model;
		_model = 0;
	}
}

