#include "GraphicClass32.h"


#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}


GraphicClass32::GraphicClass32()
{
	_D3D = NULL;
	_camera = NULL;
	_model = NULL;
	_windoModel = NULL;

	_renderTex = NULL;

	_texshader = NULL;
	_glassshader = NULL;

	power = 0;
}


GraphicClass32::~GraphicClass32()
{
}


bool GraphicClass32::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	NEW_CLASS(_model, ModelClass);
	result = _model->Initailize(GETDEVICE, "Cube2.txt", L"Texture/glass.gif", L"Texture/icenormal.gif");
	V_RETURN(result, L"Not model");

	NEW_CLASS(_windoModel, ModelClass);
	result = _windoModel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/ice.gif", L"Texture/icenormal.gif");
	V_RETURN(result, L"Not model2");

	NEW_CLASS(_renderTex, RenderTextureClass);
	V_RETURN(_renderTex->Initialize(GETDEVICE, sw, sh), L"Not RT");

	NEW_CLASS(_texshader, TextureShaderClass);
	V_RETURN(_texshader->Initialize(GETDEVICE, hwnd), L"not tex");

	NEW_CLASS(_glassshader, GlassShaderClass);
	V_RETURN(_glassshader->Initialize(GETDEVICE, hwnd), L"glass not");



	return true;
}

void GraphicClass32::ShutDown()
{
	SHUTDOWN_OBJ(_renderTex);
	SHUTDOWN_OBJ(_texshader);
	SHUTDOWN_OBJ(_glassshader);

	SHUTDOWN_OBJ(_windoModel);
	SHUTDOWN_OBJ(_model);

	SAFE_DELETE(_camera);
	SHUTDOWN_OBJ(_D3D);

}

bool GraphicClass32::Frame(D3DXVECTOR3 rotation)
{
	static float rot = 0;

	rot += (float)D3DX_PI * 0.005f;
	if (rot > 360)
	{
		rot -= 360;
	}



	_camera->SetPosition(0.f, 0.f, -10.f);

	float sintheta = sinf(D3DX_PI*rotation.y / 180) * rotation.z;
	float costheta = cosf(D3DX_PI*rotation.y / 180) * rotation.z;

	float rota = _camera->GetPosition().z + costheta;

	float rotx = _camera->GetPosition().x;
	if (rotation.z != power)
	{
		rotx += sintheta;
	}
	
	_camera->SetRotation(_camera->GetRotation().x, rotation.y, _camera->GetRotation().z);

	_camera->SetPosition(rotx, _camera->GetPosition().y, rota);

	power = rotation.z;

	if (!RenderToTexture(rot))
	{
		return false;
	}

	if (!Render(rot))
	{
		return false;
	}

	return true;
}

bool GraphicClass32::RenderToTexture(float rot)
{
	D3DXMATRIX world, view, proj;
	bool result;

	_renderTex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());

	_renderTex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	D3DXMatrixRotationY(&world, rot);

	_model->Render(_D3D->GetDeviceContext());

	result = _texshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr()[0]);
	if (!result)
	{
		return false;
	}

	_D3D->SetBackBufferRenderTarget();

	return true;

}

bool GraphicClass32::Render(float rot)
{
	D3DXMATRIX world, view, proj;

	float refractionScale;

	bool result;

	refractionScale = 0.1f;

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	D3DXMatrixRotationY(&world, rot);

	_model->Render(_D3D->GetDeviceContext());

	result = _texshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr()[0]);

	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	D3DXMatrixTranslation(&world, 0, 0, -3.5);

	_windoModel->Render(_D3D->GetDeviceContext());

	result = _glassshader->Render(_D3D->GetDeviceContext(), _windoModel->GetIndexCount(), world, view, proj, _windoModel->GetTextureArr()[0], _windoModel->GetTextureArr()[1], _renderTex->GetShaderResourceView(), refractionScale);
	if (!result)
	{
		return false;
	}

	_D3D->EndScene();

	return true;
}

