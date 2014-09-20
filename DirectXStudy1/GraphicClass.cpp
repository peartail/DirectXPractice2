#include "GraphicClass.h"


GraphicClass::GraphicClass()
{
	_D3D = NULL;

	_Camera = NULL;
	_model = NULL;
	_shader = NULL;
#ifdef __CHAPTER_SIX__
	_light = NULL;
#else
	_light = NULL;
#endif
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
#ifdef __CHATER__FOUR__
	result = _model->Initailize(_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_shader = new ColorShaderClass;
#elif defined __CHAPTER_FIVE__
	result = _model->Initailize(_D3D->GetDevice(),L"Texture/rocks_NM_height.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_shader = new TextureShaderClass;
#elif defined __CHAPTER_SIX__
	result = _model->Initailize(_D3D->GetDevice(), L"Texture/rocks_NM_height.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_shader = new LightShaderClass;
#else
	result = _model->Initailize(_D3D->GetDevice(), "Cube.txt",L"Texture/rocks_NM_height.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_shader = new LightShaderClass;
#endif


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

	_light = new LightClass;
	if (!_light)
	{
		return false;
	}

	_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void GraphicClass::ShutDown()
{

	if (_light)
	{
		delete _light;
		_light = NULL;
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

bool GraphicClass::Frame()
{
	bool result;

	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
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
	D3DXMATRIX world, view, proj;
	bool result;

	_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	D3DXVECTOR3 rot= _Camera->GetRotation();

	rot.x++;
	rot.y++;

	//_Camera->SetRotation(rot.x, rot.y, rot.z);

	_Camera->Render();

	_Camera->GetViewMatrix(view);
	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	D3DXMatrixRotationY(&world, rotation);

	_model->Render(_D3D->GetDeviceContext());

#ifdef __CHATER_FOUR__
	result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj);
#elif defined __CHAPTER_FIVE__
	result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj,_model->GetTexture());
#elif defined __CHAPTER_SIX__
	result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture(), _light->GetDirection(), _light->GetDiffuseColor());
#else 
	result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture(), _light->GetDirection(), _light->GetDiffuseColor());
#endif
	if (!result)
	{
		return false;
	}

	_D3D->EndScene();
	return true;
}