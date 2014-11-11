#include "FogShaderClass.h"


FogShaderClass::FogShaderClass()
{
	_vertexShader = NULL;
	_pixelShader = NULL;
	_layout = NULL;
	_constantBuffer = NULL;

	_sampleState = NULL;

	_fogBuffer = NULL;


	yaw = pitch = roll = posx = posy = posz = 0;
}


FogShaderClass::~FogShaderClass()
{
}



bool FogShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"fog.fx", L"fog.fx");
	if (!result)
	{
		return false;
	}
	return true;
}

void FogShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

void FogShaderClass::RotationYawPitchRoll(float y, float p, float r)
{
	yaw = y; pitch = p; roll = r;
}

void FogShaderClass::TranslationMatrix(float x, float y, float z)
{
	posx = x; posy = y; posz = z;

}
bool FogShaderClass::Render(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView* texture,float fogstart,float fogend )
{
	bool result;

	D3DXMATRIX rolworld = world;
	D3DXMATRIX transworld = world;

	D3DXMatrixRotationYawPitchRoll(&rolworld, yaw, pitch, roll);

	D3DXMatrixTranslation(&transworld, posx, posy, posz);


	D3DXMatrixMultiply(&world, &rolworld, &transworld);

	result = SetShaderParameters(context, world, view, proj, texture , fogstart,fogend);
	if (!result)
	{
		return false;
	}

	RenderShader(context, indexCount);

	return true;
}

bool FogShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsfile, WCHAR* psfile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC fogbufferDesc;


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
	matrixBufferDesc.ByteWidth = sizeof(ConstantBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_constantBuffer);

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

	fogbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	fogbufferDesc.ByteWidth = sizeof(FogBufferType);
	fogbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fogbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fogbufferDesc.MiscFlags = 0;
	fogbufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&fogbufferDesc, NULL, &_fogBuffer);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}

void FogShaderClass::ShutdownShader()
{
	if (_fogBuffer)
	{
		_fogBuffer->Release();
		_fogBuffer = NULL;
	}

	if (_sampleState)
	{
		_sampleState->Release();
		_sampleState = NULL;
	}

	if (_constantBuffer)
	{
		_constantBuffer->Release();
		_constantBuffer = NULL;
	}

	if (_layout)
	{
		_layout->Release();
		_layout = NULL;
	}

	if (_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = NULL;
	}

	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = NULL;
	}
}

void FogShaderClass::OutputShaderErrorMessage(ID3D10Blob* errormessage, HWND hwnd, WCHAR* shaderfile)
{
	char* compileError;
	unsigned long bufferSize, i;
	ofstream fout;

	compileError = (char*)(errormessage->GetBufferPointer());

	bufferSize = errormessage->GetBufferSize();

	fout.open("shader-error.txt");

	for (i = 0; i<bufferSize; i++)
	{
		fout << compileError[i];
	}

	fout.close();

	errormessage->Release();
	errormessage = NULL;

	MessageBox(hwnd, L"error compile shader,check error", shaderfile, MB_OK);
}

bool FogShaderClass::SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, ID3D11ShaderResourceView* texture,float fogstart,float fogend)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ConstantBufferType* dataPtr;
	unsigned int bufferNumber;
	FogBufferType* dataPtr2;

	result = context->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (ConstantBufferType*)mappedResource.pData;

	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->proj = proj;

	context->Unmap(_constantBuffer, 0);

	bufferNumber = 0;

	context->VSSetConstantBuffers(bufferNumber, 1, &_constantBuffer);

	//텍스쳐 설정 부문
	context->PSSetShaderResources(0, 1, &texture);

	result = context->Map(_fogBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (FogBufferType*)mappedResource.pData;

	dataPtr2->fogstart = fogstart;
	dataPtr2->fogend = fogend;

	context->Unmap(_fogBuffer, 0);

	bufferNumber = 1;

	context->VSSetConstantBuffers(bufferNumber, 1, &_fogBuffer);

	

	return true;
}

void FogShaderClass::RenderShader(ID3D11DeviceContext* context, int indexCnt)
{
	context->IASetInputLayout(_layout);

	context->VSSetShader(_vertexShader, NULL, 0);
	context->PSSetShader(_pixelShader, NULL, 0);

	context->PSSetSamplers(0, 1, &_sampleState);

	context->DrawIndexed(indexCnt, 0, 0);
}