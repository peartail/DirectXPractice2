#include "D3dclass.h"


D3dclass::D3dclass()
{
	m_swapChain = NULL;
	m_device = NULL;
	m_deviceContext = NULL;

	m_renderTargetView = NULL;
	m_depthStencilBuffer = NULL;
	m_depthStencilState = NULL;

	m_depthStencilView = NULL;
	m_rasterState = NULL;

	_depthDisabledStencilState = NULL;

	_alphaDisableBlendingState = NULL;
	_alphaEnableBlendingState = NULL;

	_screenwidth = _screenheight = 0;
}


D3dclass::~D3dclass()
{
}

bool D3dclass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	
	unsigned int numerator, denominator;
	
	_screenwidth = screenWidth;
	_screenheight = screenHeight;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	float fieldOfView, screenAspect;

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;

	m_vsync_enabled = vsync;

	if (!SettingDisplay(numerator, denominator, screenWidth, screenHeight))
	{
		return false;
	}

	//����ü�� DESC �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//����۸� 1���� ���� ����(2���� ����?)
	swapChainDesc.BufferCount = 1;

	//����� ���� �ʺ� ����
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//����۸� 32BIT �����̽��� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//������� ���ΰ�ħ ������ ����
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//������� �뵵�� ����(��)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �������� �̷���� ������ �ڵ� ����
	swapChainDesc.OutputWindow = hwnd;

	//��Ƽ���ø� ��(�̰� ����)
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//��üȭ�� ��� ����
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	//��ĵ���� ���Ŀ� ��ĵ���̴��� �������� �������� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//��µ� ������ ������� ������ ����
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//�߰� �ɼ� �÷��׸� ������� ����
	swapChainDesc.Flags = 0;

	//���ķ����� ���̷�Ʈ11�� ����
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//����ü��, Dx ����̽� Dx ����̽� ���ؽ�Ʈ ����
	//������ ���� D3D_DRIVER_TYPE_REFERENCE�� �ٲٸ� DX11 �����ȵǵ� �ȴٴµ� ���Ҹ��� ��...
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	//������� �����͸� ������
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	//������� �����ͷ� ����Ÿ�� �並 ����
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	//����� ������ �Ҵ�����
	backBufferPtr->Release();
	backBufferPtr = NULL;

	//���� ������ DESC �ʱ�ȭ
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//���� ���� DESC �ۼ�
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//description�� ����Ͽ� ���� ���� �ؽ��� ����
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//���ٽ� DESC �ʱ�ȭ
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	//���ٽ� DESC �ۼ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//���ٽ� ����Ʈ ���̽�? ����
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//���ٽ� �����̽�? ����
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//����-���ٽ� ���� ����
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	//����-���ٽ� ���� ����
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//����-���ٽ� �� �ʱ�ȭ
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//����-���ٽ� �� DESC �ۼ�
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//����-���ٽ� �� ����
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε�
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//� ������ ��� �׸��� �����ϴ� ������ȭ�� DESC �ۼ�
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	//rasterDesc.FillMode = D3D11_FILL_SOLID;
	//��ó�� ���̃m
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//�ۼ��� DESC ���κ��� ������ȭ�� ���� ����
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	//������ȭ�� ���� ����
	m_deviceContext->RSSetState(m_rasterState);

	//�������� ���� ����Ʈ ����
	_viewport.Width = (float)screenWidth;
	_viewport.Height = (float)screenHeight;
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;

	//����Ʈ ����
	m_deviceContext->RSSetViewports(1, &_viewport);

	//������� ����
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	//3D �������� ���� ������� ����
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	//���� ����� ���� ��ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&m_worldMatrix);

	//2D �������� �����(����) ��� ����
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	ZeroMemory(&depthDisabledStencilDesc,sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xff;
	depthDisabledStencilDesc.StencilWriteMask = 0xff;

	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	/*
	//���� ����Ǳ� ��
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	*/

	//���� ����ǰ� �� ��
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	result = m_device->CreateBlendState(&blendStateDescription, &_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	result = m_device->CreateBlendState(&blendStateDescription, &_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void D3dclass::Shutdown()
{
	//�����ϱ� ���� ������ ���� �ٲ����� ������ ����ü�� �Ҵ� �����Ҷ� �����߻�?
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (_alphaEnableBlendingState)
	{
		_alphaEnableBlendingState->Release();
		_alphaEnableBlendingState = NULL;
	}

	if (_alphaDisableBlendingState)
	{
		_alphaDisableBlendingState->Release();
		_alphaDisableBlendingState = NULL;
	}

	

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = NULL;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}

	if (_depthDisabledStencilState)
	{
		_depthDisabledStencilState->Release();
		_depthDisabledStencilState = NULL;

	}
	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = NULL;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = NULL;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULL;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = NULL;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = NULL;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = NULL;
	}
}

bool D3dclass::SettingDisplay(unsigned int &numerator, unsigned int &denominator, int screenWidth,int screenHeight)
{
	int error;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, stringLength;

	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	HRESULT result;


	//�ؿ������� �׳� ���󾲴°��� �ռҸ��� ��
	//DX �׷��� �������̽� ���丮 ����?
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	//���丮 ��ü�� ù��° �׷���ī�� �������̽� ����� ����
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	//���(�����)�� ���� ù��° �����
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	//DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���信 �´� ����� ������ ����
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// ������ ��� ����Ϳ� �׷���ī�� ������ �����Ҹ���Ʈ ����
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	//���÷��� ��忡 ���� ����Ʈ ������ ä������(displayModeList����?)
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	//���� ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� �˻�
	//������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���ڰ� ����
	for (i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//�����(�׷���ī��)�� DESC�� ������
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	//���� �׷���ī�� �޸𸮿뷮�� �ް�����Ʈ ������ ����
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);


	//�׷���ī�� �̸��� CHAR �� ���ڷ� ����
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	//���������� �Ҵ�����
	delete[] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = NULL;

	adapter->Release();
	adapter = NULL;

	factory->Release();
	factory = NULL;

	return true;
}

void D3dclass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	//���� ���� ����
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//����� ������ ����
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	//���� ���۸� ����
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3dclass::EndScene()
{
	//������ �Ϸ� �� ����� ���� ǥ��
	if (m_vsync_enabled)
	{
		//���ΰ�ħ ���� ����(60)
		m_swapChain->Present(1, 0);
	}
	else
	{
		//������ ������ ǥ��
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* D3dclass::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3dclass::GetDeviceContext()
{
	return m_deviceContext;
}

void D3dclass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}

void D3dclass::GetWorldMatrix(D3DXMATRIX& worldmatrix)
{
	worldmatrix = m_worldMatrix;
}

void D3dclass::GetOrthoMatrix(D3DXMATRIX& orthomatrix)
{
	orthomatrix = m_orthoMatrix;
}

void D3dclass::GetVideoCardInfo(char* cardname, int& memory)
{
	strcpy_s(cardname, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}

void D3dclass::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void D3dclass::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(_depthDisabledStencilState, 1);
}

void D3dclass::TurnOnAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void D3dclass::TurnOffAlphaBlending()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

ID3D11DepthStencilView* D3dclass::GetDepthStencilView()
{
	return m_depthStencilView;
}

void D3dclass::SetBackBufferRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void D3dclass::ResetViewport()
{
	//����Ʈ ����
	m_deviceContext->RSSetViewports(1, &_viewport);
}