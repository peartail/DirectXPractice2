#include "TeselColorShader.h"


TeselColorShader::TeselColorShader()
{
	_vertexShader = NULL;
	_pixelShader = NULL;
	_layout = NULL;
	_matrixBuffer = NULL;

	_tessellationBuffer = NULL;
	_hullshader = NULL;
	_domainshader = NULL;
}


TeselColorShader::~TeselColorShader()
{
}


bool TeselColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"Shader\\teselcolor.fx", L"Shader\\teselcolor.fx", L"Shader\\teselcolor.fx", L"Shader\\teselcolor.fx");
	if (!result)
	{
		return false;
	}
	return true;
}

void TeselColorShader::Shutdown()
{
	ShutdownShader();
}

bool TeselColorShader::Render(ID3D11DeviceContext* context, int indexCount, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj,float tesselationAmount)
{
	bool result;

	result = SetShaderParameters(context, world, view, proj, tesselationAmount);
	if (!result)
	{
		return false;
	}

	RenderShader(context, indexCount);

	return true;
}

bool TeselColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* hsfile, WCHAR* dsfile, WCHAR* psfilename)
{
	HRESULT result;

	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;

	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	D3D11_BUFFER_DESC tessellationBufferDesc;

	//������ NULL ����
	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	hullShaderBuffer = NULL;
	domainShaderBuffer = NULL;
	pixelShaderBuffer = NULL;

	//���� ���̴� ������
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//�� ���̴� ������
	result = D3DX11CompileFromFile(hsfile, NULL, NULL, "CHS", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &hullShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, hsfile);
		}
		else
		{
			MessageBox(hwnd, hsfile, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//������ ���̴� ������
	result = D3DX11CompileFromFile(dsfile, NULL, NULL, "CDS", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &domainShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, dsfile);
		}
		else
		{
			MessageBox(hwnd, dsfile, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//�ʼ� ���̴� ������
	result = D3DX11CompileFromFile(psfilename, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psfilename);
		}
		else
		{
			MessageBox(hwnd, psfilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//���� ���̴� ����
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &_hullshader);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &_domainshader);
	if (FAILED(result))
	{
		return false;
	}

	//�ȼ� ���̴� ����
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//LAYOUT DESC �ۼ�
	//ModelClass �� VertexType �� ��ġ�ؾ���
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//���̾ƿ� ��� ������ ������?
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//�� ���⼭ �����Է� ���̾ƿ� �������ؼ� �׷�
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(result))
	{
		return false;
	}

	//���� ó��
	vertexShaderBuffer->Release();
	vertexShaderBuffer = NULL;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	//�������̴� ��� ��� ���� DESC �ۼ�
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;


	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� �����A ���ְ� �Ѵٴ°� �ռҸ�??
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	tessellationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tessellationBufferDesc.ByteWidth = sizeof(TessellationBufferType);
	tessellationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tessellationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tessellationBufferDesc.MiscFlags = 0;
	tessellationBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&tessellationBufferDesc, NULL, &_tessellationBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TeselColorShader::ShutdownShader()
{
	if (_tessellationBuffer)
	{
		_tessellationBuffer->Release();
		_tessellationBuffer = NULL;
	}

	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = NULL;
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

	if (_domainshader)
	{
		_domainshader->Release();
		_domainshader = NULL;
	}

	if (_hullshader)
	{
		_hullshader->Release();
		_hullshader = NULL;
	}

	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = NULL;
	}
}

void TeselColorShader::OutputShaderErrorMessage(ID3D10Blob* errormessage, HWND hwnd, WCHAR* shaderfilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errormessage->GetBufferPointer());

	bufferSize = errormessage->GetBufferSize();

	fout.open("shader_error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errormessage->Release();
	errormessage = NULL;

	MessageBox(hwnd, L"Error compile Shader, Check Shader err", shaderfilename, MB_OK);
}

bool TeselColorShader::SetShaderParameters(ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj,float teselamt)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	TessellationBufferType* dataPtr2;
	unsigned int bufferNumber;

	// ����� transpose(��ġ���) �ؼ� ���̴����� ����� �� �ְ��Ѵٴµ� �װ� ����?
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&proj, &proj);

	//��������� ������ �����ְ� ���?
	result = context->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	//��������� �������� �����͸� ������
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��� ���ۿ� ��� ����
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = proj;

	//��� ����
	context->Unmap(_matrixBuffer, 0);

	//�������̴����� ��������� ��ġ ����
	bufferNumber = 0;

	//�������̴��� ������۸� �ٲﰪ���� �ٲ�
	//context->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	context->DSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	result = context->Map(_tessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (TessellationBufferType*)mappedResource.pData;

	dataPtr2->tesselAmount = teselamt;

	dataPtr2->padding = D3DXVECTOR3(0, 0, 0);

	context->Unmap(_tessellationBuffer, 0);

	bufferNumber = 1;

	context->HSSetConstantBuffers(bufferNumber, 1, &_tessellationBuffer);

	return true;


}

void TeselColorShader::RenderShader(ID3D11DeviceContext* context, int indexcnt)
{
	context->IASetInputLayout(_layout);

	context->VSSetShader(_vertexShader, NULL, 0);
	context->HSSetShader(_hullshader, NULL, 0);
	context->DSSetShader(_domainshader, NULL, 0);
	context->PSSetShader(_pixelShader, NULL, 0);

	context->DrawIndexed(indexcnt, 0, 0);
}