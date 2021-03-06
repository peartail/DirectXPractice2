#include "WaterShaderClass.h"


WaterShaderClass::WaterShaderClass()
{
	TextureShaderClass::TextureShaderClass();
	_reflectionBuffer = NULL;
	_waterBuffer = NULL;
}


WaterShaderClass::~WaterShaderClass()
{
}

bool WaterShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"water.fx", L"water.fx");
	if (!result)
	{
		return false;
	}
	return true;
}


bool WaterShaderClass::Render(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX refmat, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* refract, ID3D11ShaderResourceView* normaltex, float waterTrans, float reflectRefractScale)
{
	bool result;

	result = SetShaderParameters(context, world, view, proj, refmat, texture, refract, normaltex, waterTrans, reflectRefractScale);
	if (!result)
	{
		return false;
	}

	RenderShader(context, indexCount);

	return true;
}

bool WaterShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfile, WCHAR* psfile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC relectionBufferDesc;
	D3D11_BUFFER_DESC waterBufferDesc;

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
	
	relectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	relectionBufferDesc.ByteWidth = sizeof(ReflectionBufferType);
	relectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	relectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	relectionBufferDesc.MiscFlags = 0;
	relectionBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&relectionBufferDesc, NULL, &_reflectionBuffer);
	if (FAILED(result))
	{
		return false;
	}

	waterBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	waterBufferDesc.ByteWidth = sizeof(WaterBufferType);
	waterBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	waterBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	waterBufferDesc.MiscFlags = 0;
	waterBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&waterBufferDesc, NULL, &_waterBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void WaterShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

void WaterShaderClass::ShutdownShader()
{
	if (_waterBuffer)
	{
		_waterBuffer->Release();
		_waterBuffer = NULL;
	}

	if (_reflectionBuffer)
	{
		_reflectionBuffer->Release();
		_reflectionBuffer = NULL;
	}

	TextureShaderClass::ShutdownShader();
}

bool WaterShaderClass::SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX refmat,ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* refractT, ID3D11ShaderResourceView* normalT, float waterT, float refrefrScale)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	ReflectionBufferType* dataPtr2;
	WaterBufferType* dataPtr3;

	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);
	D3DXMatrixTranspose(&refmat, &refmat);

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



	result = context->Map(_reflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (ReflectionBufferType*)mappedResource.pData;

	dataPtr2->reflection = refmat;

	context->Unmap(_reflectionBuffer, 0);

	bufferNumber = 1;

	context->VSSetConstantBuffers(bufferNumber, 1, &_reflectionBuffer);

	//텍스쳐 설정 부문
	context->PSSetShaderResources(0, 1, &texture);

	context->PSSetShaderResources(1, 1, &refractT);

	context->PSSetShaderResources(2, 1, &normalT);

	result = context->Map(_waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (WaterBufferType*)mappedResource.pData;

	dataPtr3->waterTranslation = waterT;
	dataPtr3->reflectRefractScale = refrefrScale;
	dataPtr3->padding = D3DXVECTOR2(0, 0);

	context->Unmap(_waterBuffer, 0);

	bufferNumber = 2;

	context->PSSetConstantBuffers(bufferNumber, 1, &_waterBuffer);

	return true;
}
