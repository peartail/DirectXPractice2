#include "GraphicClass27.h"

#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}

GraphicClass27::GraphicClass27()
{
	_D3D = NULL;
	_camera = NULL;
	_model = NULL;
	_texShader = NULL;
	_rendertex = NULL;
	_floor = NULL;
	_refshader = NULL;
}


GraphicClass27::~GraphicClass27()
{
}

bool GraphicClass27::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");
	
	NEW_CLASS(_camera, CameraClass);
	NEW_CLASS(_model, ModelClass);

	result = _model->Initailize(GETDEVICE, "Cube2.txt", L"Texture/rocks.jpg");
	V_RETURN(result, L"Not model");

	NEW_CLASS(_texShader, TextureShaderClass);
	V_RETURN(_texShader->Initialize(GETDEVICE, hwnd),L"Not texshader");

	NEW_CLASS(_rendertex, RenderTextureClass);
	V_RETURN(_rendertex->Initialize(GETDEVICE, sw, sh),L"Not rendertex");

	NEW_CLASS(_floor, ModelClass);
	V_RETURN(_floor->Initailize(GETDEVICE, "plane.txt", L"Texture/clouds.bmp"), L"Not floor");

	NEW_CLASS(_refshader, ReflectionShaderClass);
	V_RETURN(_refshader->Initialize(GETDEVICE, hwnd), L"Not Refshader");

	return true;


}

void GraphicClass27::ShutDown()
{
	SHUTDOWN_OBJ(_refshader);
	SHUTDOWN_OBJ(_floor);
	SHUTDOWN_OBJ(_rendertex);
	SHUTDOWN_OBJ(_texShader);
	SHUTDOWN_OBJ(_model);

	SAFE_DELETE(_camera);
	SHUTDOWN_OBJ(_D3D);
}

bool GraphicClass27::Frame()
{
	_camera->SetPosition(0,-1 , -2.f);
	_camera->SetRotation(10, 0, 0);
	Render();
	return true;
}

bool GraphicClass27::Render()
{
	bool result;

	result = RenderToTexture();
	IS_V(result);

	IS_V(RenderScene());

	return true;
}

bool GraphicClass27::RenderToTexture()
{
	D3DXMATRIX world, refv, proj;
	static float rot = 0.f;

	_rendertex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());

	_rendertex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0.f, 0.f, 0.f, 1.f);

	_camera->RenderReflection(-1.5f);

	refv = _camera->GetReflectionViewMatrix();

	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	rot += (float)D3DX_PI*0.005f;

	if (rot > 360.f)
	{
		rot -= 360;
	}

	

	D3DXMATRIX tempworld = world, scaleworld = world,rota = world;
	
	D3DXMatrixRotationY(&world, rot);

	//D3DXMatrixScaling(&scaleworld, 10.0, 10.0, 10.0);

	//D3DXMatrixMultiply(&world, &scaleworld, &rota);

	_model->Render(_D3D->GetDeviceContext());

	_texShader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, refv, proj, _model->GetTexture());

	_D3D->SetBackBufferRenderTarget();

	return true;

}


bool GraphicClass27::RenderScene()
{
	D3DXMATRIX world, view, proj, refmat;
	bool result;
	static float rot = 0.0f;

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	rot += (float)D3DX_PI* 0.005f;
	if (rot > 360.f)
	{
		rot -= 360;
	}

	D3DXMatrixRotationY(&world, rot);

	_model->Render(_D3D->GetDeviceContext());

	result = _texShader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture());
	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	D3DXMATRIX tempworld = world, scaleworld = world;

	D3DXMatrixTranslation(&world, 0.f, -1.5f, 0.f);

	//D3DXMatrixScaling(&scaleworld, 10.0, 10.0, 10.0);

	//D3DXMatrixMultiply(&world, &scaleworld, &tempworld);

	refmat = _camera->GetReflectionViewMatrix();

	_floor->Render(_D3D->GetDeviceContext());

	result = _refshader->Render(_D3D->GetDeviceContext(), _floor->GetIndexCount(), world, view, proj, _floor->GetTexture(), _rendertex->GetShaderResourceView(), refmat);

	_D3D->EndScene();

	return true;
}









