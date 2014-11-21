#pragma once

#ifndef __D3DCLASS_H_
#define __D3DCLASS_H_

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dx10.lib")

#include "dxgi.h"
#include "d3dcommon.h"
#include "d3d11.h"
#include "d3dx10math.h"

class D3dclass
{
public:
	D3dclass();
	D3dclass(const D3dclass&);
	~D3dclass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetBackBufferRenderTarget();

	void ResetViewport();
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;

	ID3D11DepthStencilState* _depthDisabledStencilState;

	ID3D11BlendState* _alphaEnableBlendingState;
	ID3D11BlendState* _alphaDisableBlendingState;

	int _screenwidth, _screenheight;

	D3D11_VIEWPORT _viewport;

private:
	bool SettingDisplay(unsigned int &numerator, unsigned int &denominator, int screenWidth, int screenHeight);
};

#endif