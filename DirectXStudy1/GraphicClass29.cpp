#include "GraphicClass29.h"

#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}


GraphicClass29::GraphicClass29()
{
	_D3D = NULL;
	_camera = NULL;
	_wallmodel = _bathmodel = _watermodel = _groundmodel = NULL;
	_reftex = _refractex = NULL;
	_light = NULL;
	_lightshader = NULL;
	_refractShader = NULL;
	_watershader = NULL;
}


GraphicClass29::~GraphicClass29()
{
}

bool GraphicClass29::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	NEW_CLASS(_groundmodel, ModelClass);
	result = _groundmodel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/clouds.bmp");
	V_RETURN(result, L"Not model");

	NEW_CLASS(_wallmodel, ModelClass);
	V_RETURN(_wallmodel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/clouds.bmp"), L"Not model");

	NEW_CLASS(_bathmodel, ModelClass);
	V_RETURN(_bathmodel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/clouds.bmp"), L"Not model");

	NEW_CLASS(_watermodel, ModelClass);
	V_RETURN(_watermodel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/water.gif"), L"Not model");

	NEW_CLASS(_light, LightClass);
	_light->SetAmbientColor(0.15, 0.15, 0.15, 1);
	_light->SetDiffuseColor(1, 1, 1, 1);
	_light->SetDirection(0, -1, 0.5f);
	
	NEW_CLASS(_refractex, RenderTextureClass);
	V_RETURN(_refractex->Initialize(GETDEVICE, sw, sh), L"Not Refract");

	NEW_CLASS(_reftex, RenderTextureClass);
	V_RETURN(_reftex->Initialize(GETDEVICE, sw, sh), L"Not rendertex");


	NEW_CLASS(_lightshader, LightShaderClass);
	V_RETURN(_lightshader->Initialize(GETDEVICE, hwnd),L"Not Lightshader");

	NEW_CLASS(_refractShader, RefractionShaderClass);
	V_RETURN(_refractShader->Initialize(GETDEVICE, hwnd),L"Not refshader");

	NEW_CLASS(_watershader, WaterShaderClass);
	V_RETURN(_watershader->Initialize(GETDEVICE, hwnd),L"Not Watershader");

	_waterh = 2.75f;
	_waterTrans = 0.f;
	

	return true;
}

void GraphicClass29::ShutDown()
{
	SHUTDOWN_OBJ(_watershader);
	SHUTDOWN_OBJ(_refractShader);
	SHUTDOWN_OBJ(_lightshader);
	SHUTDOWN_OBJ(_refractex);
	SHUTDOWN_OBJ(_reftex);

	SAFE_DELETE(_light);

	SHUTDOWN_OBJ(_watermodel);
	SHUTDOWN_OBJ(_bathmodel);
	SHUTDOWN_OBJ(_wallmodel);
	SHUTDOWN_OBJ(_groundmodel);

	SAFE_DELETE(_camera);

	SHUTDOWN_OBJ(_D3D);

}

bool GraphicClass29::Frame(D3DXVECTOR3 rotation)
{
	_waterTrans += 0.001;
	if (_waterTrans > 1.0f)
	{
		_waterTrans -= 1.f;
	}
	


	_camera->SetPosition(0.f, 2.f, -3.f);
	_camera->SetRotation(0.f, 45, 0);

	_camera->SetRotation(_camera->GetRotation().x, rotation.y, _camera->GetRotation().z);
	_camera->SetPosition(_camera->GetPosition().x, _camera->GetPosition().y, rotation.z);

	if (!Render())
	{
		return false;
	}

	return true;
}

bool GraphicClass29::Render()
{
	bool result;

	result = RenderRefractionToTexture();
	if (!result)
	{
		return false;
	}

	result = RenderReflectionToTexture();
	if (!result)
	{
		return false;
	}

	result = RenderScene();
	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicClass29::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX world, view, proj;

	bool result;

	clipPlane = D3DXVECTOR4(0, -1, 0, _waterh + 0.1f);

	_refractex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());

	_refractex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(),0,0,0,1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	D3DXMatrixTranslation(&world, 0, 2, 0);

	//_bathmodel->Render(_D3D->GetDeviceContext());

	//IS_V(_refractShader->Render(_D3D->GetDeviceContext(), _bathmodel->GetIndexCount(), world, view, proj, _bathmodel->GetTexture(), _light->GetDirection(), _light->GetAmbientColor(), _light->GetDiffuseColor(), clipPlane));	

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass29::RenderReflectionToTexture()
{
	D3DXMATRIX refview, world, proj;
	bool result;

	_reftex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());

	_reftex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0, 0, 0, 1);

	_camera->RenderReflection(_waterh);

	refview = _camera->GetReflectionViewMatrix();

	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	D3DXMatrixTranslation(&world, 0, 6, 8);

	//_wallmodel->Render(_D3D->GetDeviceContext());

	//IS_V(_lightshader->Render(_D3D->GetDeviceContext(), _wallmodel->GetIndexCount(), world, refview, proj, _wallmodel->GetTexture(), _light->GetDirection(), _light->GetAmbientColor(), _light->GetDiffuseColor(), _camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 0.15f));

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass29::RenderScene()
{
	D3DXMATRIX world, view, proj, refmat;
	bool result;

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	D3DXMatrixTranslation(&world, 0, 1, 0);

	//_groundmodel->Render(_D3D->GetDeviceContext());

	//IS_V(_lightshader->Render(_D3D->GetDeviceContext(), _groundmodel->GetIndexCount(), world, view, proj, _groundmodel->GetTexture(), _light->GetDirection(), _light->GetAmbientColor(), _light->GetDiffuseColor(), _camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 0.15f));	

	_D3D->GetWorldMatrix(world);

	D3DXMatrixTranslation(&world, 0, 6, 8);

	//_wallmodel->Render(_D3D->GetDeviceContext());

	//IS_V(_lightshader->Render(_D3D->GetDeviceContext(), _wallmodel->GetIndexCount(), world, view, proj, _wallmodel->GetTexture(), _light->GetDirection(), _light->GetAmbientColor(), _light->GetDiffuseColor(), _camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 0.15f));

	_D3D->GetWorldMatrix(world);

	D3DXMatrixTranslation(&world, 0, 2, 0);

	//_bathmodel->Render(_D3D->GetDeviceContext());

	//IS_V(_lightshader->Render(_D3D->GetDeviceContext(), _bathmodel->GetIndexCount(), world, view, proj, _bathmodel->GetTexture(), _light->GetDirection(), _light->GetAmbientColor(), _light->GetDiffuseColor(), _camera->GetPosition(), D3DXVECTOR4(1, 1, 1, 1), 0.15f));


	_D3D->GetWorldMatrix(world);

	refmat = _camera->GetReflectionViewMatrix();

	D3DXMatrixTranslation(&world, 0, _waterh, 0);

	_watermodel->Render(_D3D->GetDeviceContext());

	result = _watershader->Render(_D3D->GetDeviceContext(), _watermodel->GetIndexCount(), world, view, proj, refmat, _reftex->GetShaderResourceView(), _refractex->GetShaderResourceView(), _watermodel->GetTexture(), _waterTrans, 0.01);

	if (!result)
	{
		return false;
	}

	_D3D->EndScene();

	return true;


}