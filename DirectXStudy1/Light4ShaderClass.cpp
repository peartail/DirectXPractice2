#include "Light4ShaderClass.h"


Light4ShaderClass::Light4ShaderClass()
{
	TextureShaderClass::TextureShaderClass();

	_lightColorBuffer = NULL;
	_lightPosBuffer = NULL;
}


Light4ShaderClass::~Light4ShaderClass()
{
}


bool Light4ShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"light4.fx", L"light4.fx");
	if (!result)
	{
		return false;
	}
	return true;
}


bool Light4ShaderClass::Render(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView* texture, D3DXVECTOR4 difColor[],D3DXVECTOR4 lightPos[])
{
	bool result;

	result = SetShaderParameters(context, world, view, proj, texture, difColor,lightPos);
	if (!result)
	{
		return false;
	}

	RenderShader(context, indexCount);

	return true;
}

bool Light4ShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfile, WCHAR* psfile)
{
  	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;

	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightColorBufferDesc;
	D3D11_BUFFER_DESC lightPositionBufferDesc;


	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;

	result = D3DX11CompileFromFile(vsfile, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsfile);
		}
		else
		{
			MessageBox(hwnd, vsfile, L"MissingSahderFile", MB_OK);
		}
		return false;
	}

	result = D3DX11CompileFromFile(psfile, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psfile);
		}
		else
		{
			MessageBox(hwnd, psfile, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if (FAILED(result))
	{
		return false;
	}


	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = NULL;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);

	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	lightColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightColorBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	lightColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightColorBufferDesc.MiscFlags = 0;
	lightColorBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightColorBufferDesc, NULL, &_lightColorBuffer);
	if (FAILED(result))
	{
		return false;
	}

	lightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightPositionBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	lightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightPositionBufferDesc.MiscFlags = 0;
	lightPositionBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightPositionBufferDesc, NULL, &_lightPosBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Light4ShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

void Light4ShaderClass::ShutdownShader()
{
	if (_lightColorBuffer)
	{
		_lightColorBuffer->Release();
		_lightColorBuffer = NULL;
	}

	if (_lightPosBuffer)
	{
		_lightPosBuffer->Release();
		_lightPosBuffer = NULL;
	}

	TextureShaderClass::ShutdownShader();
}

bool Light4ShaderClass::SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj,  ID3D11ShaderResourceView* texture, D3DXVECTOR4 difcolor[],D3DXVECTOR4 lightpos[])
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	LightPositionBufferType* dataPtr2;
	LightColorBufferType* dataPtr3;

	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	result = context->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	context->Unmap(_matrixBuffer, 0);

	bufferNumber = 0;

	context->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);



	result = context->Map(_lightPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (LightPositionBufferType*)mappedResource.pData;

	dataPtr2->lightPos[0] = lightpos[0];
	dataPtr2->lightPos[1] = lightpos[1];
	dataPtr2->lightPos[2] = lightpos[2];
	dataPtr2->lightPos[3] = lightpos[3];

	context->Unmap(_lightPosBuffer, 0);

	bufferNumber = 1;

	context->VSSetConstantBuffers(bufferNumber, 1, &_lightPosBuffer);

	//텍스쳐 설정 부문
	context->PSSetShaderResources(0, 1, &texture);

	result = context->Map(_lightColorBuffer , 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (LightColorBufferType*)mappedResource.pData;

	dataPtr3->difColor[0] = difcolor[0];
	dataPtr3->difColor[1] = difcolor[1];
	dataPtr3->difColor[2] = difcolor[2];
	dataPtr3->difColor[3] = difcolor[3];

	context->Unmap(_lightColorBuffer, 0);

	bufferNumber = 2;

	context->PSSetConstantBuffers(bufferNumber, 1, &_lightColorBuffer);

	return true;
}
