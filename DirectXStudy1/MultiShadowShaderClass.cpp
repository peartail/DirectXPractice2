#include "MultiShadowShaderClass.h"


MultiShadowShaderClass::MultiShadowShaderClass()
{
	_vertexShader = NULL;
	_pixelShader = NULL;
	_layout = NULL;
	_sampleStateClamp = _sampleStateWrap = NULL;

	_matrixBuffer = NULL;
	_lightBuffer = _lightBuffer2 = NULL;
}


MultiShadowShaderClass::~MultiShadowShaderClass()
{
}

bool MultiShadowShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"Shader\\multishadow.fx", L"Shader\\multishadow.fx");
	if (!result)
	{
		return false;
	}
	return true;
}

void MultiShadowShaderClass::Shutdown()
{
	ShutdownShader();
}


bool MultiShadowShaderClass::Render(ID3D11DeviceContext* context, int indexCnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX lightview, D3DXMATRIX lightproj, ID3D11ShaderResourceView* tex, ID3D11ShaderResourceView* depthtex, D3DXVECTOR3 lightpos, D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse,D3DXMATRIX lightViewmat2,D3DXMATRIX lightprojmat2,ID3D11ShaderResourceView* depthmapTex2,D3DXVECTOR3 lightpos2,D3DXVECTOR4 difColor2)
{
	bool result;

	result = SetShaderParameters(context, world, view, proj, lightview, lightproj, tex, depthtex, lightpos, ambient, diffuse,lightViewmat2,lightprojmat2,depthmapTex2,lightpos2,difColor2);


	if (!result)
	{
		return false;
	}
	RenderShader(context, indexCnt);

	return true;
}

bool MultiShadowShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFile, WCHAR* psFile)
{
	HRESULT result;
	ID3D10Blob* errormsg;
	ID3D10Blob* vertexShader;
	ID3D10Blob* pixelShader;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;

	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc2;

	errormsg = NULL;
	vertexShader = NULL;
	pixelShader = NULL;

	result = D3DX11CompileFromFile(vsFile, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShader, &errormsg, NULL);
	if (FAILED(result))
	{
		if (errormsg)
		{
			OutputShaderErrorMessage(errormsg, hwnd, vsFile);
		}
		else
		{
			MessageBox(hwnd, vsFile, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	result = D3DX11CompileFromFile(psFile, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL
		, &pixelShader, &errormsg, NULL);
	if (FAILED(result))
	{
		if (errormsg)
		{
			OutputShaderErrorMessage(errormsg, hwnd, psFile);
		}
		else
		{
			MessageBox(hwnd, psFile, L"Missing ps shader", MB_OK);
		}
		return false;
	}

	result = device->CreateVertexShader(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), NULL, &_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(), NULL, &_pixelShader);
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

	result = device->CreateInputLayout(polygonLayout, numElements, vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), &_layout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShader->Release();
	vertexShader = NULL;

	pixelShader->Release();
	pixelShader = NULL;

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

	result = device->CreateSamplerState(&samplerDesc, &_sampleStateWrap);
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	result = device->CreateSamplerState(&samplerDesc, &_sampleStateClamp);
	if (FAILED(result))
	{
		return false;
	}

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

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc2, NULL, &_lightBuffer2);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void MultiShadowShaderClass::ShutdownShader()
{
	if (_lightBuffer2)
	{
		_lightBuffer2->Release();
		_lightBuffer2 = NULL;
	}

	if (_lightBuffer)
	{
		_lightBuffer->Release();
		_lightBuffer = NULL;
	}



	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = NULL;
	}

	if (_sampleStateWrap)
	{
		_sampleStateWrap->Release();
		_sampleStateWrap = NULL;
	}

	if (_sampleStateClamp)
	{
		_sampleStateClamp->Release();
		_sampleStateClamp = NULL;
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

void MultiShadowShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMEssage, HWND hwnd, WCHAR* filename)
{
	char* compileError;
	unsigned long bufferSize, i;
	ofstream fout;

	compileError = (char*)(errorMEssage->GetBufferPointer());

	bufferSize = errorMEssage->GetBufferSize();

	fout.open("shader-error.txt");

	for (i = 0; i<bufferSize; i++)
	{
		fout << compileError[i];
	}

	fout.close();

	errorMEssage->Release();
	errorMEssage = NULL;

	MessageBox(hwnd, L"error compile shader,check error", filename, MB_OK);
}

bool MultiShadowShaderClass::SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, D3DXMATRIX lightview, D3DXMATRIX lightproj, ID3D11ShaderResourceView* tex, ID3D11ShaderResourceView* depthtex, D3DXVECTOR3 lightpos, D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse,D3DXMATRIX lightview2,D3DXMATRIX lightproj2,ID3D11ShaderResourceView* depthtex2,D3DXVECTOR3 lightpos2,D3DXVECTOR4 difcolor2)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNum;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	LightBufferType2* dataPtr3;

	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	D3DXMatrixTranspose(&lightview, &lightview);
	D3DXMatrixTranspose(&lightproj, &lightproj);

	D3DXMatrixTranspose(&lightview2, &lightview2);
	D3DXMatrixTranspose(&lightproj2, &lightproj2);

	result = context->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->proj = proj;

	dataPtr->lightView = lightview;
	dataPtr->lightProj = lightproj;

	dataPtr->lightView2 = lightview2;
	dataPtr->lightPorj2 = lightproj2;

	context->Unmap(_matrixBuffer, 0);

	bufferNum = 0;

	context->VSSetConstantBuffers(bufferNum, 1, &_matrixBuffer);

	context->PSSetShaderResources(0, 1, &tex);

	context->PSSetShaderResources(1, 1, &depthtex);

	context->PSSetShaderResources(2, 1, &depthtex2);

	result = context->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor = ambient;
	dataPtr2->diffueseColor = diffuse;
	dataPtr2->diffueseColor2 = difcolor2;

	context->Unmap(_lightBuffer, 0);

	bufferNum = 1;

	context->PSSetConstantBuffers(bufferNum, 1, &_lightBuffer);

	result = context->Map(_lightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (LightBufferType2*)mappedResource.pData;

	dataPtr3->lightPos = lightpos;
	dataPtr3->padding = 0.0f;
	dataPtr3->lightPos2 = lightpos2;
	dataPtr3->padding2 = 0.0f;

	context->Unmap(_lightBuffer2, 0);

	bufferNum = 2;

	context->VSSetConstantBuffers(bufferNum, 1, &_lightBuffer2);

	return true;
}

void MultiShadowShaderClass::RenderShader(ID3D11DeviceContext* context, int indexCnt)
{
	context->IASetInputLayout(_layout);

	context->VSSetShader(_vertexShader, NULL, 0);
	context->PSSetShader(_pixelShader, NULL, 0);

	context->PSSetSamplers(0, 1, &_sampleStateClamp);
	context->PSSetSamplers(1, 1, &_sampleStateWrap);

	context->DrawIndexed(indexCnt, 0, 0);
}
