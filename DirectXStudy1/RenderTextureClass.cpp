#include "RenderTextureClass.h"


RenderTextureClass::RenderTextureClass()
{
	_renderTargetTexture = NULL;
	_renderTargetView = NULL;
	_shaderResourceView = NULL;
}


RenderTextureClass::~RenderTextureClass()
{
}

bool RenderTextureClass::Initialize(ID3D11Device* device, int texwidth, int texheight)
{
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

void RenderTextureClass::Shutdown()
{
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



void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthstencilview)
{
	context->OMSetRenderTargets(1, &_renderTargetView, depthstencilview);
}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthstencilview, float r, float g, float b, float a)
{
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	context->ClearRenderTargetView(_renderTargetView, color);

	context->ClearDepthStencilView(depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return _shaderResourceView;
}