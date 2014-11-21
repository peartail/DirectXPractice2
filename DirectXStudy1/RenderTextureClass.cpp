#include "RenderTextureClass.h"


RenderTextureClass::RenderTextureClass()
{
	_renderTargetTexture = NULL;
	_renderTargetView = NULL;
	_shaderResourceView = NULL;

	_texwidth = _texheight = 0;
	_depthstencilview = NULL; 

	_depthStencilBuffer = NULL;
}


RenderTextureClass::~RenderTextureClass()
{
}

bool RenderTextureClass::Initialize(ID3D11Device* device, int texwidth, int texheight)
{
	_texheight = texheight;
	_texwidth = texwidth;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetviewdesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceviewdesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = texwidth;
	textureDesc.Height = texheight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDesc, NULL, &_renderTargetTexture);
	if (FAILED(result))
	{
		return false;
	}

	renderTargetviewdesc.Format = textureDesc.Format;
	renderTargetviewdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetviewdesc.Texture2D.MipSlice = 0;

	result = device->CreateRenderTargetView(_renderTargetTexture, &renderTargetviewdesc, &_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	shaderResourceviewdesc.Format = textureDesc.Format;
	shaderResourceviewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceviewdesc.Texture2D.MostDetailedMip = 0;
	shaderResourceviewdesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(_renderTargetTexture, &shaderResourceviewdesc, &_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool RenderTextureClass::Initialize(ID3D11Device* device, int texwidth, int texheight, float screenDepth, float screenNear)
{
	_texheight = texheight;
	_texwidth = texwidth;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetviewdesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceviewdesc;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = texwidth;
	textureDesc.Height = texheight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDesc, NULL, &_renderTargetTexture);
	if (FAILED(result))
	{
		return false;
	}

	renderTargetviewdesc.Format = textureDesc.Format;
	renderTargetviewdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetviewdesc.Texture2D.MipSlice = 0;

	result = device->CreateRenderTargetView(_renderTargetTexture, &renderTargetviewdesc, &_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	shaderResourceviewdesc.Format = textureDesc.Format;
	shaderResourceviewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceviewdesc.Texture2D.MostDetailedMip = 0;
	shaderResourceviewdesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(_renderTargetTexture, &shaderResourceviewdesc, &_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	//36장 이후로 추가된 것... 제대로 동작하려나...

	//깊이 버퍼의 DESC 초기화
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//깊이 버퍼 DESC 작성
	depthBufferDesc.Width = texwidth;
	depthBufferDesc.Height = texheight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//description을 사용하여 깊이 버퍼 텍스쳐 생성
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}
	//깊이-스텐실 뷰 초기화
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//깊이-스텐실 뷰 DESC 작성
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//깊이-스텐실 뷰 생성
	result = device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthstencilview);
	if (FAILED(result))
	{
		return false;
	}

	_viewport.Width = (float)texwidth;
	_viewport.Height = (float)texheight;
	_viewport.MinDepth = 0;
	_viewport.MaxDepth = 1;
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;

	D3DXMatrixPerspectiveFovLH(&_projectionMatrix, ((float)D3DX_PI / 4.0), ((float)texwidth / (float)texheight),screenNear,screenDepth);

	//2D 렌더링용 정사용(오쏘) 행렬 생성
	D3DXMatrixOrthoLH(&_ortho, (float)texwidth, (float)texheight, screenNear, screenDepth);

	return true;

}

void RenderTextureClass::Shutdown()
{
	if (_depthstencilview)
	{
		_depthstencilview->Release();
		_depthstencilview = NULL;
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = NULL;
	}

	if (_shaderResourceView)
	{
		_shaderResourceView->Release();
		_shaderResourceView = NULL;
	}

	if (_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = NULL;
	}

	if (_renderTargetTexture)
	{
		_renderTargetTexture->Release();
		_renderTargetTexture = NULL;
	}
}



void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* context)
{
	context->OMSetRenderTargets(1, &_renderTargetView, _depthstencilview);

	context->RSSetViewports(1, &_viewport);
}

void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* depth)
{
	context->OMSetRenderTargets(1, &_renderTargetView, depth);
}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* depth, float r, float g, float b, float a)
{
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	context->ClearRenderTargetView(_renderTargetView, color);

	context->ClearDepthStencilView(depth, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* context, float r, float g, float b, float a)
{
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	context->ClearRenderTargetView(_renderTargetView, color);

	context->ClearDepthStencilView(_depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);

}



ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return _shaderResourceView;
}

int RenderTextureClass::GetTextureHeight()
{
	return _texheight;
}

int RenderTextureClass::GetTextureWidth()
{
	return _texwidth;
}

void RenderTextureClass::GetOrthoMatrix(D3DXMATRIX& ortho)
{
	ortho = _ortho;
}