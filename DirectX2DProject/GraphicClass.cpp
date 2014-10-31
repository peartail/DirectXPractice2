#include "GraphicClass.h"



GraphicClass::GraphicClass()
{
	_D3D = NULL;

	_Camera = NULL;
	_model = NULL;
	_shader = NULL;

	_bitmap = NULL;
	_text = NULL;
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
	D3DXMATRIX baseViewMatrix;

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

	_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	_Camera->Render();
	_Camera->GetViewMatrix(baseViewMatrix);

	_text = new TextClass;
	if (!_text)
	{
		return false;
	}

	result = _text->Initialize(_D3D->GetDevice(), _D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"CNITTX", L"Error", MB_OK);
		return false;
	}
	
	_shader = new TextureShaderClass;
	if (!_shader)
	{
		return false;
	}

	result = _shader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Coud not init colorshader", L"Error", MB_OK);
		return false;
	}
	

	_bitmap = new BitmapClass;
	if (!_bitmap)
	{
		return false;
	}

	//result = _bitmap->Initialize(_D3D->GetDevice(), screenWidth, screenHeight, L"Texture/rocks_NM_height.dds", 256, 256);
	result = _bitmap->Initialize(_D3D->GetDevice(), screenWidth, screenHeight, L"Texture/128.png", 12, 12);
	if (!result)
	{
		MessageBox(hwnd, L"Coud not init bitmap obj", L"ERror", MB_OK);
	}
	
	return true;
}

void GraphicClass::ShutDown()
{

	/*if (_light)
	{
		delete _light;
		_light = NULL;
	}*/
	if (_text)
	{
		_text->Shutdown();
		delete _text;
		_text = NULL;
	}
	if (_bitmap)
	{
		_bitmap->Shutdown();
		delete _bitmap;
		_bitmap = NULL;
	}

	if (_shader)
	{
		_shader->Shutdown();
		delete _shader;
		_shader = NULL;
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

bool GraphicClass::Frame(int mouseX, int mouseY, int fps, int cpu, float ftime)
{
	bool result;

	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	mX = mouseX;
	mY = mouseY;
	result = _text->SetMousePosition(mouseX, mouseY, _D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = _text->SetFps(fps, _D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = _text->SetCpu(cpu, _D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicClass::Render(float rotation)
{
	D3DXMATRIX world, view, proj,ortho;
	bool result;

	_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	_Camera->Render();

	_Camera->GetViewMatrix(view);
	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	_D3D->GetOrthoMatrix(ortho);

	//D3DXMatrixRotationY(&world, rotation);
	//D3DXMatrixRotationYawPitchRoll(&world, rotation, rotation, rotation);
	
	//_model->Render(_D3D->GetDeviceContext());


	_D3D->TurnZBufferOff();
	
	
	result = _bitmap->Render(_D3D->GetDeviceContext(), mX, mY);
	if (!result)
	{
		return false;
	}
	result = _shader->Render(_D3D->GetDeviceContext(), _bitmap->GetIndexCount(), world, view, ortho, _bitmap->GetTexture());
	
	_D3D->TurnOnAlphaBlending();

	result = _text->Render(_D3D->GetDeviceContext(), world, ortho);
	if (!result)
	{
		return false;
	}
	
	_D3D->TurnOffAlphaBlending();

	_D3D->TurnZBufferOn();

	_D3D->EndScene();
	return true;
}