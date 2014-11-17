#include "FireShaderClass.h"


FireShaderClass::FireShaderClass()
{
	TextureShaderClass::TextureShaderClass();
	_noiseBuffer = NULL;

	_sampleState2 = NULL;
	_distortBuffer = NULL;
}


FireShaderClass::~FireShaderClass()
{
}


bool FireShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L".\\Shader\\fire.fx", L".\\Shader\\fire.fx");
	if (!result)
	{
		return false;
	}
	return true;
}


bool FireShaderClass::Render(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView* firetex, ID3D11ShaderResourceView* noisetex, ID3D11ShaderResourceView* alphatex, float frametime,D3DXVECTOR3 scrollspeed,D3DXVECTOR3 scales,D3DXVECTOR2 dist1,D3DXVECTOR2 dist2,D3DXVECTOR2 dist3,float distscale,float distbias)
{
	bool result;

	result = SetShaderParameters(context, world, view, proj, firetex, noisetex, alphatex, frametime, scrollspeed, scales, dist1, dist2, dist3, distscale, distbias);
	if (!result)
	{
		return false;
	}

	RenderShader(context, indexCount);

	return true;
}

bool FireShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfile, WCHAR* psfile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_SAMPLER_DESC samplerDesc2;

	D3D11_BUFFER_DESC noiseBufferDesc;
	D3D11_BUFFER_DESC distortBufferDesc;

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

	samplerDesc2.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc2.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc2.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc2.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc2.MipLODBias = 0.0f;
	samplerDesc2.MaxAnisotropy = 1;
	samplerDesc2.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc2.BorderColor[0] = 0;
	samplerDesc2.BorderColor[1] = 0;
	samplerDesc2.BorderColor[2] = 0;
	samplerDesc2.BorderColor[3] = 0;
	samplerDesc2.MinLOD = 0;
	samplerDesc2.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc2, &_sampleState2);
	if (FAILED(result))
	{
		return false;
	}

	noiseBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	noiseBufferDesc.ByteWidth = sizeof(NoiseBufferType);
	noiseBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	noiseBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	noiseBufferDesc.MiscFlags = 0;
	noiseBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&noiseBufferDesc, NULL, &_noiseBuffer);
	if (FAILED(result))
	{
		return false;
	}

	distortBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	distortBufferDesc.ByteWidth = sizeof(DistortionBufferType);
	distortBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	distortBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	distortBufferDesc.MiscFlags = 0;
	distortBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&distortBufferDesc, NULL, &_distortBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FireShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

void FireShaderClass::ShutdownShader()
{

	if (_distortBuffer)
	{
		_distortBuffer->Release();
		_distortBuffer = NULL;
	}

	if (_sampleState2)
	{
		_sampleState2->Release();
		_sampleState2 = NULL;
	}
	if (_noiseBuffer)
	{
		_noiseBuffer->Release();
		_noiseBuffer = NULL;
	}

	TextureShaderClass::ShutdownShader();
}

bool FireShaderClass::SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView* firetex, ID3D11ShaderResourceView* noisetex, ID3D11ShaderResourceView* alphatex, float frametime, D3DXVECTOR3 scrollspeed, D3DXVECTOR3 scales, D3DXVECTOR2 dist1, D3DXVECTOR2 dist2, D3DXVECTOR2 dist3, float distscale, float distbias)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	NoiseBufferType* dataPtr2;
	DistortionBufferType* dataPtr3;
	unsigned int bufferNumber;

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

	

	result = context->Map(_noiseBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (NoiseBufferType*)mappedResource.pData;

	dataPtr2->frameTime = frametime;
	dataPtr2->scrollSpeed = scrollspeed;
	dataPtr2->scale = scales;
	dataPtr2->padding = 0;

	context->Unmap(_noiseBuffer, 0);

	bufferNumber = 1;

	context->VSSetConstantBuffers(bufferNumber, 1, &_noiseBuffer);



	//텍스쳐 설정 부문
	context->PSSetShaderResources(0, 1, &firetex);
	context->PSSetShaderResources(1, 1, &noisetex);

	context->PSSetShaderResources(2, 1, &alphatex);

	result = context->Map(_distortBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (DistortionBufferType*)mappedResource.pData;

	dataPtr3->dist1 = dist1;
	dataPtr3->dist2 = dist2;
	dataPtr3->dist3 = dist3;
	dataPtr3->distScale = distscale;
	dataPtr3->distBias = distbias;

	context->Unmap(_distortBuffer, 0);

	bufferNumber = 2;

	context->PSSetConstantBuffers(bufferNumber, 1, &_distortBuffer);

	return true;
}

void FireShaderClass::RenderShader(ID3D11DeviceContext* context, int indexcnt)
{
	context->IASetInputLayout(_layout);

	context->VSSetShader(_vertexShader, NULL, 0);
	context->PSSetShader(_pixelShader, NULL, 0);

	context->PSSetSamplers(0, 1, &_sampleState);

	context->PSSetSamplers(1, 1, &_sampleState2);

	context->DrawIndexed(indexcnt, 0, 0);
}