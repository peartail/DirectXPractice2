#include "GraphicClass39.h"

#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}



GraphicClass39::GraphicClass39()
{
	_D3D = NULL;
	_camera = NULL;

	_particleShader = NULL;
	_particleSystem = NULL;
}


GraphicClass39::~GraphicClass39()
{
}


bool GraphicClass39::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	_camera->SetPosition(0, -2, -10);

	NEW_CLASS(_particleShader, ParticleShaderClass);
	V_RETURN(_particleShader->Initialize(GETDEVICE, hwnd),L"N Paritlce");

	NEW_CLASS(_particleSystem, ParticleSystemClass);
	V_RETURN(_particleSystem->Initialize(GETDEVICE, L"Texture/icon_star.png"), L"N Paritlce");
	

	return true;
}

void GraphicClass39::ShutDown()
{
	SHUTDOWN_OBJ(_particleSystem);
	SHUTDOWN_OBJ(_particleShader);
	SAFE_DELETE(_camera);
	SHUTDOWN_OBJ(_D3D);

}

bool GraphicClass39::Frame(D3DXVECTOR3 rotation,int ftime)
{
	static float rot = 0;

	rot += (float)D3DX_PI * 0.005f;
	if (rot > 360)
	{
		rot -= 360;
	}

	float rota = _camera->GetPosition().z + rotation.z;
	float rotx = _camera->GetPosition().x + rotation.x;



	_camera->SetRotation(_camera->GetRotation().x, rotation.y, _camera->GetRotation().z);
	_camera->SetPosition(rotx, _camera->GetPosition().y, rota);

	_particleSystem->Frame(ftime, _D3D->GetDeviceContext());

	if (!Render(rot))
	{
		return false;
	}

	return true;
}

bool GraphicClass39::Render(float rot)
{
	D3DXMATRIX world, view, proj, translate;
	bool result;
	D3DXVECTOR3 cameraPos, modelPos;
	double angle;
	float rotation;

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	_D3D->TurnOnAlphaBlending();

	_particleSystem->Render(_D3D->GetDeviceContext());

	IS_V(_particleShader->Render(_D3D->GetDeviceContext(), _particleSystem->GetIndexCount(), world, view, proj, _particleSystem->GetTexture()));

	_D3D->TurnOffAlphaBlending();

	_D3D->EndScene();

	return true;
}

