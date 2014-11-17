#include "GraphicClass34.h"

#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}

GraphicClass34::GraphicClass34()
{
	_D3D = NULL;
	_camera = NULL;
	_textureShader = NULL;

	_floorModel = NULL;
	_billboardModel = NULL;
}


GraphicClass34::~GraphicClass34()
{
}


bool GraphicClass34::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	NEW_CLASS(_floorModel, ModelClass);

	V_RETURN(_floorModel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/firetex.gif"), L"Not floor");

	NEW_CLASS(_billboardModel, ModelClass);

	V_RETURN(_billboardModel->Initailize(GETDEVICE, "Cube2.txt", L"Texture/firetex.gif"), L" Not billboard");



	return true;
}

void GraphicClass34::ShutDown()
{
	SHUTDOWN_OBJ(_billboardModel);
	SHUTDOWN_OBJ(_floorModel);

	SAFE_DELETE(_camera);
	SHUTDOWN_OBJ(_D3D);

}

bool GraphicClass34::Frame(D3DXVECTOR3 rotation)
{
	static float rot = 0;

	rot += (float)D3DX_PI * 0.005f;
	if (rot > 360)
	{
		rot -= 360;
	}

	_camera->SetPosition(0.f, 0.f, -10.f);

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

bool GraphicClass34::Render(float rot)
{
	D3DXMATRIX world, view, proj,translate;
	bool result;
	D3DXVECTOR3 cameraPos, modelPos;
	double angle;
	float rotation;

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	_floorModel->Render(_D3D->GetDeviceContext());
	IS_V(_textureShader->Render(_D3D->GetDeviceContext(), _floorModel->GetIndexCount(), world, view, proj, _floorModel->GetTexture()));

	cameraPos = _camera->GetPosition();

	modelPos.x = 0;
	modelPos.y = 1.5;
	modelPos.z = 0;

	angle = atan2(modelPos.x - cameraPos.x, modelPos.z - cameraPos.z)*(180 / D3DX_PI);

	rotation = (float)angle * 0.0174532925f;

	D3DXMatrixRotationY(&world, rotation);

	D3DXMatrixTranslation(&translate, modelPos.x, modelPos.y, modelPos.z);

	D3DXMatrixMultiply(&world, &world, &translate);

	_billboardModel->Render(_D3D->GetDeviceContext());
	IS_V(_textureShader->Render(_D3D->GetDeviceContext(), _billboardModel->GetIndexCount(), world, view, proj, _billboardModel->GetTexture()));



	_D3D->EndScene();

	return true;
}

