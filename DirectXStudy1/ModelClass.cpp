#include "ModelClass.h"


ModelClass::ModelClass(void)
{
	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;

	_texture = NULL;

	_texarr = NULL;

	_model = NULL;
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass(void)
{
}

bool ModelClass::Initailize(ID3D11Device* device,char* modelfilename, WCHAR* texturefilename)
{
	bool result;

	result = LoadModel(modelfilename);
	if(!result)
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

bool ModelClass::Initailize(ID3D11Device* device, char* modelfilename, WCHAR* tex1,WCHAR* tex2)
{
	bool result;

	result = LoadModel(modelfilename);
	if (!result)
	{
		return false;
	}

	CalculateModelVectors();

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, tex1,tex2);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::Initailize(ID3D11Device* device, char* modelfilename, WCHAR* tex1, WCHAR* tex2,WCHAR* tex3)
{
	bool result;

	result = LoadModel(modelfilename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, tex1, tex2,tex3);
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
	ReleaseModel();
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

ID3D11ShaderResourceView** ModelClass::GetTextureArr()
{
	return _texarr->GetTextureArray();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	
	VertexType* vertice;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc,indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData,indexData;
	HRESULT result;
	
	int i;
	
	//배열 생성
	vertice = new VertexType[m_vertexCount];
	if (!vertice)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	for(i=0;i<m_vertexCount;i++)
	{
		vertice[i].pos = D3DXVECTOR3(_model[i].x,_model[i].y,_model[i].z);
		vertice[i].texture = D3DXVECTOR2(_model[i].tu,_model[i].tv);
		vertice[i].normal = D3DXVECTOR3(_model[i].nx,_model[i].ny,_model[i].nz);
		vertice[i].tangent = D3DXVECTOR3(_model[i].tx, _model[i].ty, _model[i].tz);
		vertice[i].binorm = D3DXVECTOR3(_model[i].bx, _model[i].by, _model[i].bz);

		indices[i] = i;
	}


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

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* tex1,WCHAR* tex2)
{
	bool result;

	_texarr = new TextureArray;
	if (!_texarr)
	{
		return false;
	}

	result = _texarr->Initialize(device, tex1,tex2);
	if (!result)
	{
		return false;
	}
	return true;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* tex1, WCHAR* tex2,WCHAR* tex3)
{
	bool result;

	_texarr = new TextureArray;
	if (!_texarr)
	{
		return false;
	}

	result = _texarr->Initialize(device, tex1, tex2,tex3);
	if (!result)
	{
		return false;
	}
	return true;
}

void ModelClass::ReleaseTexture()
{
	if (_texarr)
	{
		_texarr->Shutdown();
		delete _texarr;
		_texarr = NULL;
	}

	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = NULL;
	}
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if(fin.fail())
	{
		return false;
	}

	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	_model = new ModelType[m_vertexCount];
	if(!_model)
	{
		return false;
	}

	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for(i=0;i<m_vertexCount;i++)
	{
		fin >> _model[i].x >> _model[i].y >> _model[i].z;
		fin >> _model[i].tu >> _model[i].tv;
		fin >> _model[i].nx >> _model[i].ny >> _model[i].nz;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if(_model)
	{
		delete [] _model;
		_model = 0;
	}
}


void ModelClass::CalculateModelVectors()
{
	int faceCnt, i, index;
	TempVertexType v1, v2, v3;
	VectorType tan, bin, nor;

	faceCnt = m_vertexCount / 3;

	index = 0;

	for (i = 0; i < faceCnt; i++)
	{
		v1.x = _model[index].x;
		v1.y = _model[index].y;
		v1.z = _model[index].z;
		v1.tu = _model[index].tu;
		v1.tv = _model[index].tv;
		v1.nx = _model[index].nx;
		v1.ny = _model[index].ny;
		v1.nz = _model[index].nz;
		index++;

		v2.x = _model[index].x;
		v2.y = _model[index].y;
		v2.z = _model[index].z;
		v2.tu = _model[index].tu;
		v2.tv = _model[index].tv;
		v2.nx = _model[index].nx;
		v2.ny = _model[index].ny;
		v2.nz = _model[index].nz;
		index++;

		v3.x = _model[index].x;
		v3.y = _model[index].y;
		v3.z = _model[index].z;
		v3.tu = _model[index].tu;
		v3.tv = _model[index].tv;
		v3.nx = _model[index].nx;
		v3.ny = _model[index].ny;
		v3.nz = _model[index].nz;
		index++;

		CalculateTangentBinormal(v1, v2, v3, tan, bin);

		CalculateNormal(tan, bin, nor);

		_model[index - 1].nx = nor.x;
		_model[index - 1].ny = nor.y;
		_model[index - 1].nz = nor.z;
		_model[index - 1].tx = tan.x;
		_model[index - 1].ty = tan.y;
		_model[index - 1].tz = tan.z;
		_model[index - 1].bx = bin.x;
		_model[index - 1].by = bin.y;
		_model[index - 1].bz = bin.z;

		_model[index - 2].nx = nor.x;
		_model[index - 2].ny = nor.y;
		_model[index - 2].nz = nor.z;
		_model[index - 2].tx = tan.x;
		_model[index - 2].ty = tan.y;
		_model[index - 2].tz = tan.z;
		_model[index - 2].bx = bin.x;
		_model[index - 2].by = bin.y;
		_model[index - 2].bz = bin.z;

		_model[index - 3].nx = nor.x;
		_model[index - 3].ny = nor.y;
		_model[index - 3].nz = nor.z;
		_model[index - 3].tx = tan.x;
		_model[index - 3].ty = tan.y;
		_model[index - 3].tz = tan.z;
		_model[index - 3].bx = bin.x;
		_model[index - 3].by = bin.y;
		_model[index - 3].bz = bin.z;
	}
}


void ModelClass::CalculateTangentBinormal(TempVertexType v1, TempVertexType v2, TempVertexType v3,VectorType& tan,VectorType& bin)
{
	float vec1[3], vec2[3];
	float tuvec[2], tvvec[2];
	float den;
	float length;

	vec1[0] = v2.x - v1.x;
	vec1[1] = v2.y - v1.y;
	vec1[2] = v2.z - v1.z;

	vec2[0] = v3.x - v1.x;
	vec2[1] = v3.y - v1.y;
	vec2[2] = v3.z - v1.z;

	tuvec[0] = v2.tu - v1.tu;
	tvvec[0] = v2.tv - v1.tv;

	tuvec[1] = v3.tu - v1.tu;
	tvvec[1] = v3.tv - v1.tv;

	den = 1.0f / (tuvec[0] * tvvec[1] - tuvec[1] * tvvec[0]);

	tan.x = (tvvec[1] * vec1[0] - tvvec[0] * vec2[0]) * den;
	tan.y = (tvvec[1] * vec1[1] - tvvec[0] * vec2[1]) * den;
	tan.z = (tvvec[1] * vec1[2] - tvvec[0] * vec2[2]) * den;

	bin.x = (tuvec[0] * vec2[0] - tuvec[1] * vec1[0]) * den;
	bin.y = (tuvec[0] * vec2[1] - tuvec[1] * vec1[1]) * den;
	bin.z = (tuvec[0] * vec2[2] - tuvec[1] * vec1[2]) * den;

	length = sqrt((tan.x * tan.x) + (tan.y * tan.y) + (tan.z*tan.z));

	tan.x = tan.x / length;
	tan.y = tan.y / length;
	tan.z = tan.z / length;

	length = sqrt((bin.x * bin.x) + (bin.y*bin.y) + (bin.z*bin.z));

	bin.x = bin.x / length;
	bin.y = bin.y / length;
	bin.z = bin.z / length;
}

void ModelClass::CalculateNormal(VectorType tan, VectorType bin, VectorType& nor)
{
	float length;

	nor.x = (tan.y * bin.z) - (tan.z * bin.y);
	nor.y = (tan.z * bin.x) - (tan.x * bin.z);
	nor.z = (tan.x * bin.y) - (tan.y * bin.x);

	length = sqrt((nor.x * nor.x) + (nor.y*nor.y) + (nor.z * nor.z));

	nor.x = nor.x / length;
	nor.y = nor.y / length;
	nor.z = nor.z / length;
}