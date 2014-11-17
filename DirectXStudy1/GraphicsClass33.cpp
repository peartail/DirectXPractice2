#include "GraphicsClass33.h"

#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}

GraphicsClass33::GraphicsClass33()
{
	_D3D = NULL;
	_camera = NULL;
	_model = NULL;
	
	_fireShader = NULL;

	_floorModel = NULL;
	_billboardModel = NULL;
}


GraphicsClass33::~GraphicsClass33()
{
}

bool GraphicsClass33::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	NEW_CLASS(_model, ModelClass);
	result = _model->Initailize(GETDEVICE, "Cube2.txt", L"Texture/firetex.gif", L"Texture/firenormal.gif",L"Texture/firealpha.gif");
	V_RETURN(result, L"Not model");

	NEW_CLASS(_fireShader, FireShaderClass);
	result = _fireShader->Initialize(GETDEVICE,hwnd);
	V_RETURN(result, L"Not fire");

	NEW_CLASS(_floorModel, ModelClass);

	V_RETURN(_floorModel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/firetex.gif"), L"Not floor");

	NEW_CLASS(_billboardModel, ModelClass);

	V_RETURN(_billboardModel->Initailize(GETDEVICE,"Cube2.txt", L"Texture/firetex.gif"), L" Not billboard");



	return true;
}

void GraphicsClass33::ShutDown()
{
	SHUTDOWN_OBJ(_billboardModel);
	SHUTDOWN_OBJ(_floorModel);

	SHUTDOWN_OBJ(_fireShader);
	SHUTDOWN_OBJ(_model);

	SAFE_DELETE(_camera);
	SHUTDOWN_OBJ(_D3D);

}

bool GraphicsClass33::Frame(D3DXVECTOR3 rotation)
{
	static float rot = 0;

	rot += (float)D3DX_PI * 0.005f;
	if (rot > 360)
	{
		rot -= 360;
	}

	_camera->SetPosition(0.f, 0.f, -10.f);

	//float sintheta = sinf(D3DX_PI*rotation.y / 180) * rotation.z;
	//float costheta = cosf(D3DX_PI*rotation.y / 180) * rotation.z;

	float rota = _camera->GetPosition().z + rotation.z;
	float rotx = _camera->GetPosition().x + rotation.x;



	_camera->SetRotation(_camera->GetRotation().x, rotation.y, _camera->GetRotation().z);
	_camera->SetPosition(rotx, _camera->GetPosition().y, rota);

	

	if (!Render(rot))
	{
		return false;
	}

	return true;
}

bool GraphicsClass33::Render(float rot)
{
	D3DXMATRIX world, view, proj;
	bool result;

	D3DXVECTOR3 scrollSpeed, scales;
	D3DXVECTOR2 dist1, dist2, dist3;
	float distortScale, distortBias;
	static float frameTime = 0.f;

	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0;
	}

	scrollSpeed = D3DXVECTOR3(1.3f, 2.1f, 2.3f);

	scales = D3DXVECTOR3(1, 2, 3);

	dist1 = D3DXVECTOR2(0.1f, 0.2f);
	dist2 = D3DXVECTOR2(0.1f, 0.3f);
	dist3 = D3DXVECTOR2(0.1f, 0.1f);

	distortScale = 0.8f;
	distortBias = 0.5f;

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	_D3D->TurnOnAlphaBlending();

	_model->Render(_D3D->GetDeviceContext());

	result = _fireShader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr()[0], _model->GetTextureArr()[1], _model->GetTextureArr()[2], frameTime, scrollSpeed, scales, dist1, dist2, dist3, distortScale, distortBias);

	if (!result)
	{
		return false;
	}

	_D3D->TurnOffAlphaBlending();


	_D3D->EndScene();

	return true;
}

