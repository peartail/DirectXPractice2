#include "GraphicClass41.h"

#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}


GraphicClass41::GraphicClass41()
{
	_D3D = NULL;
	_camera = NULL;
	_cubeModel = _spehereModel = _groundModel = NULL;

	_light = NULL;
	_renderTex = NULL;
	_depthshader = NULL;
	_shadowshader = NULL;

	_light2 = NULL;
	_renderTex2 = NULL;
}


GraphicClass41::~GraphicClass41()
{
}

bool GraphicClass41::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	_camera->SetPosition(0, 2, -10);

	NEW_CLASS(_cubeModel, ModelClass);

	V_RETURN(_cubeModel->Initailize(GETDEVICE, "Models\\Cube2.txt", L"Texture/wall01.dds"), L"Not floor1");

	_cubeModel->SetPosition(-2.f, 2.f, 0.f);

	NEW_CLASS(_spehereModel, ModelClass);

	V_RETURN(_spehereModel->Initailize(GETDEVICE, "Models\\sphere.txt", L"Texture/ice.dds"), L"Not floor2");

	_spehereModel->SetPosition(2, 2, 0);

	NEW_CLASS(_groundModel, ModelClass);

	V_RETURN(_groundModel->Initailize(GETDEVICE, "Models\\plane01.txt", L"Texture/metal001.dds"), L"Not floor3");

	_groundModel->SetPosition(0, 1, 0);

	NEW_CLASS(_light, LightClass);

	_light->SetAmbientColor(0.15, 0.15, 0.15, 1.f);
	_light->SetDiffuseColor(1, 1, 1, 1);

	_light->SetLookAt(0, 0, 0);
	_light->GenerateProjcetionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	NEW_CLASS(_renderTex, RenderTextureClass);
	V_RETURN(_renderTex->Initialize(GETDEVICE, SHADOWMAP_WIDTH, SHADOWMAP_WEIGHT, SCREEN_DEPTH, SCREEN_NEAR), L"Not rt");

	NEW_CLASS(_depthshader, DepthShaderClass);
	V_RETURN(_depthshader->Initialize(GETDEVICE, hwnd), L"Not depth");

	NEW_CLASS(_shadowshader, MultiShadowShaderClass);
	V_RETURN(_shadowshader->Initialize(GETDEVICE, hwnd), L"Not depth");

	NEW_CLASS(_light2, LightClass);
	
	_light2->SetDiffuseColor(1, 1, 1, 1);
	_light2->SetLookAt(0, 0, 0);
	_light2->GenerateProjcetionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	NEW_CLASS(_renderTex2, RenderTextureClass);

	V_RETURN(_renderTex2->Initialize(GETDEVICE, SHADOWMAP_WIDTH, SHADOWMAP_WEIGHT, SCREEN_DEPTH, SCREEN_NEAR), L"not rt2");

	return true;
}

void GraphicClass41::ShutDown()
{
	SHUTDOWN_OBJ(_renderTex2);
	SAFE_DELETE(_light2);
	SAFE_DELETE(_light);

	SHUTDOWN_OBJ(_shadowshader);
	SHUTDOWN_OBJ(_depthshader);
	SHUTDOWN_OBJ(_renderTex);

	SHUTDOWN_OBJ(_groundModel);
	SHUTDOWN_OBJ(_spehereModel);
	SHUTDOWN_OBJ(_cubeModel);

	SAFE_DELETE(_camera);
	SHUTDOWN_OBJ(_D3D);

}

bool GraphicClass41::Frame(D3DXVECTOR3 rotation, float x, float y, float z, float rotx, float roty, float rotz)
{
	bool result;

	static float lightPosx = -5.f;

	_camera->SetPosition(x, y, z);
	_camera->SetRotation(rotx, roty, rotz);

	_light->SetPosition(5, 8, -5);
	_light2->SetPosition(-5, 8, -5);

	/*lightPosx += 0.05f;
	if (lightPosx > 5)
	{
		lightPosx = -5;
	}*/

	//_light->SetPosition(lightPosx, 8, -5);

	if (!Render())
	{
		return false;
	}

	return true;
}

bool GraphicClass41::RenderSceneToTexture()
{
	D3DXMATRIX world, lightview, lightproj, translatemat;
	float posx, posy, posz;
	bool result;

	_renderTex->SetRenderTarget(_D3D->GetDeviceContext());
	_renderTex->ClearRenderTarget(_D3D->GetDeviceContext(), 0, 0, 0, 1);

	_light->GenerateViewMatrix();

	_D3D->GetWorldMatrix(world);
	_light->GetViewMatrix(lightview);
	_light->GetProjMatrix(lightproj);

	_cubeModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);

	_cubeModel->Render(_D3D->GetDeviceContext());
	result = _depthshader->Render(_D3D->GetDeviceContext(), _cubeModel->GetIndexCount(), world, lightview, lightproj);
	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	_spehereModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);

	_spehereModel->Render(_D3D->GetDeviceContext());
	result = _depthshader->Render(_D3D->GetDeviceContext(), _spehereModel->GetIndexCount(), world, lightview, lightproj);
	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	_groundModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);

	_groundModel->Render(_D3D->GetDeviceContext());
	result = _depthshader->Render(_D3D->GetDeviceContext(), _groundModel->GetIndexCount(), world, lightview, lightproj);
	if (!result)
	{
		return false;
	}

	_D3D->SetBackBufferRenderTarget();

	_D3D->ResetViewport();

	return true;
}

bool GraphicClass41::RenderSceneToTexture2()
{
	D3DXMATRIX world, lightview, lightproj, translatemat;
	float posx, posy, posz;
	bool result;

	_renderTex2->SetRenderTarget(_D3D->GetDeviceContext());
	_renderTex2->ClearRenderTarget(_D3D->GetDeviceContext(), 0, 0, 0, 1);

	_light2->GenerateViewMatrix();

	_D3D->GetWorldMatrix(world);
	_light2->GetViewMatrix(lightview);
	_light2->GetProjMatrix(lightproj);

	_cubeModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);

	_cubeModel->Render(_D3D->GetDeviceContext());
	result = _depthshader->Render(_D3D->GetDeviceContext(), _cubeModel->GetIndexCount(), world, lightview, lightproj);
	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	_spehereModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);

	_spehereModel->Render(_D3D->GetDeviceContext());
	result = _depthshader->Render(_D3D->GetDeviceContext(), _spehereModel->GetIndexCount(), world, lightview, lightproj);
	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	_groundModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);

	_groundModel->Render(_D3D->GetDeviceContext());
	result = _depthshader->Render(_D3D->GetDeviceContext(), _groundModel->GetIndexCount(), world, lightview, lightproj);
	if (!result)
	{
		return false;
	}

	_D3D->SetBackBufferRenderTarget();

	_D3D->ResetViewport();

	return true;
}

bool GraphicClass41::Render()
{
	D3DXMATRIX world, view, proj, translate;
	D3DXMATRIX lightview, lightproj;
	D3DXMATRIX lightview2, lightproj2;
	bool result;

	float posx, posy, posz;

	if (!RenderSceneToTexture())
	{
		return false;
	}

	if (!RenderSceneToTexture2())
	{
		return false;
	}

	//
	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_light->GenerateViewMatrix();

	_light2->GenerateViewMatrix();

	_camera->GetViewMatrix(view);
	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	_light->GetViewMatrix(lightview);
	_light->GetProjMatrix(lightproj);

	_light2->GetViewMatrix(lightview2);
	_light2->GetProjMatrix(lightproj2);

	_cubeModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);


	D3DXVECTOR3 lightpos(_light->GetPosition().x, _light->GetPosition().y, _light->GetPosition().z);
	D3DXVECTOR3 lightpos2(_light2->GetPosition().x, _light2->GetPosition().y, _light2->GetPosition().z);

	_cubeModel->Render(_D3D->GetDeviceContext());
	result = _shadowshader->Render(_D3D->GetDeviceContext(), _cubeModel->GetIndexCount(), world, view, proj, lightview, lightproj, _cubeModel->GetTexture(), _renderTex->GetShaderResourceView(), lightpos, _light->GetAmbientColor(), _light->GetDiffuseColor(), lightview2, lightproj2, _renderTex2->GetShaderResourceView(), lightpos2,_light2->GetDiffuseColor());

	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	_spehereModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);


	_spehereModel->Render(_D3D->GetDeviceContext());

	result = _shadowshader->Render(_D3D->GetDeviceContext(), _spehereModel->GetIndexCount(), world, view, proj, lightview, lightproj, _spehereModel->GetTexture(), _renderTex->GetShaderResourceView(), lightpos, _light->GetAmbientColor(), _light->GetDiffuseColor(), lightview2, lightproj2, _renderTex2->GetShaderResourceView(), lightpos2, _light2->GetDiffuseColor());

	if (!result)
	{
		return false;
	}

	_D3D->GetWorldMatrix(world);

	_groundModel->GetPosition(posx, posy, posz);
	D3DXMatrixTranslation(&world, posx, posy, posz);


	_groundModel->Render(_D3D->GetDeviceContext());
	result = _shadowshader->Render(_D3D->GetDeviceContext(), _groundModel->GetIndexCount(), world, view, proj, lightview, lightproj, _groundModel->GetTexture(), _renderTex->GetShaderResourceView(), lightpos, _light->GetAmbientColor(), _light->GetDiffuseColor(), lightview2, lightproj2, _renderTex2->GetShaderResourceView(), lightpos2, _light2->GetDiffuseColor());

	if (!result)
	{
		return false;
	}

	_D3D->EndScene();

	return true;
}

