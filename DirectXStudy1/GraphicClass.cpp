#include "GraphicClass.h"


GraphicClass::GraphicClass()
{
	_D3D = NULL;

	_Camera = NULL;
	_model = NULL;
	_ColorShader = NULL;
}

GraphicClass::GraphicClass(const GraphicClass& other)
{

}

GraphicClass::~GraphicClass()
{
}

bool GraphicClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	_D3D = new D3dclass;
	if (!_D3D)
	{
		return false;
	}

	result = _D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Not Init D3D", L"ERROR", MB_OK);
		return false;
	}

	_Camera = new CameraClass;
	if (!_Camera)
	{
		return false;
	}

	_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	_model = new ModelClass;
	if (!_model)
	{
		return false;
	}

	result = _model->Initailize(_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_ColorShader = new ColorShaderClass;
	if (!_ColorShader)
	{
		return false;
	}

	result = _ColorShader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Coud not init colorshader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicClass::ShutDown()
{
	if (_ColorShader)
	{
		_ColorShader->Shutdown();
		delete _ColorShader;
		_ColorShader = NULL;
	}

	if (_model)
	{
		_model->Shutdown();
		delete _model;
		_model = NULL;
	}

	if (_Camera)
	{
		delete _Camera;
		_Camera = NULL;
	}

	if (_D3D)
	{
		_D3D->Shutdown();
		delete _D3D;
		_D3D = NULL;
	}
}

bool GraphicClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicClass::Render()
{
	D3DXMATRIX world, view, proj;
	bool result;


	_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	_Camera->Render();

	_Camera->GetViewMatrix(view);
	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	_model->Render(_D3D->GetDeviceContext());

	result = _ColorShader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj);
	if (!result)
	{
		return false;
	}

	_D3D->EndScene();
	return true;
}