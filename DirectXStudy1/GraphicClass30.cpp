#include "GraphicClass30.h"

#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}

GraphicClass30::GraphicClass30()
{
	_D3D = NULL;
	_camera = NULL;
	_model = NULL;
	_lightshader = NULL;
	
	_light1 = _light2 = _light3 = _light4 = NULL;
}


GraphicClass30::~GraphicClass30()
{
}


bool GraphicClass30::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	NEW_CLASS(_model, ModelClass);
	result = _model->Initailize(GETDEVICE, "plane.txt", L"Texture/block.gif");
	V_RETURN(result, L"Not model");

	NEW_CLASS(_lightshader, Light4ShaderClass);
	V_RETURN(_lightshader->Initialize(GETDEVICE, hwnd), L"Not Lightshader");


	NEW_CLASS(_light1, LightClass);
	_light1->SetDiffuseColor(1, 0, 0, 1);
	_light1->SetPosition(-0.3, 0.3, 3);

	NEW_CLASS(_light2, LightClass);
	_light2->SetDiffuseColor(0, 1, 0, 1);
	_light2->SetPosition(0.3, 0.3, 3);

	NEW_CLASS(_light3, LightClass);
	_light3->SetDiffuseColor(0, 0, 1, 1);
	_light3->SetPosition(-0.5, -0.5, -3);

	NEW_CLASS(_light4, LightClass);
	_light4->SetDiffuseColor(1, 1, 1, 1);
	_light4->SetPosition(0.5, -0.5, -3);

	return true;
}

void GraphicClass30::ShutDown()
{

	SHUTDOWN_OBJ(_lightshader);


	SAFE_DELETE(_light1);
	SAFE_DELETE(_light2);
	SAFE_DELETE(_light3);
	SAFE_DELETE(_light4);

	SHUTDOWN_OBJ(_model);

	SAFE_DELETE(_camera);

	SHUTDOWN_OBJ(_D3D);

}

bool GraphicClass30::Frame(D3DXVECTOR3 rotation)
{

	_camera->SetPosition(0.f, 0.f, -7.f);

	float rota = _camera->GetPosition().z + rotation.z;

	_camera->SetRotation(_camera->GetRotation().x, rotation.y, _camera->GetRotation().z);
	_camera->SetPosition(_camera->GetPosition().x, _camera->GetPosition().y, rota);

	if (!Render())
	{
		return false;
	}

	return true;
}

bool GraphicClass30::Render()
{
	D3DXMATRIX world, view, proj;

	D3DXVECTOR4 difColor[4];
	D3DXVECTOR4 lPos[4];

	bool result;

	difColor[0] = _light1->GetDiffuseColor();
	difColor[1] = _light2->GetDiffuseColor();
	difColor[2] = _light3->GetDiffuseColor();
	difColor[3] = _light4->GetDiffuseColor();
	

	lPos[0] = _light1->GetPosition();
	lPos[1] = _light2->GetPosition();
	lPos[2] = _light3->GetPosition();
	lPos[3] = _light4->GetPosition();

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	_model->Render(_D3D->GetDeviceContext());

	result = _lightshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture(), difColor, lPos);
	if (!result)
	{
		return false;
	}

	_D3D->EndScene();

	return true;
}

