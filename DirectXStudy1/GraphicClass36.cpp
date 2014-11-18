#include "GraphicClass36.h"


#define NEW_CLASS(x,y) {x = new y; if(!x) return false;}
#define V_RETURN(x,y) { if(!x){MessageBox(hwnd,y,L"Err",MB_OK);return false;}}
#define IS_V(x) {if(!x) {return false;}}
#define GETDEVICE _D3D->GetDevice()
#define SHUTDOWN_OBJ(x) {if(x) {x->Shutdown();delete x; x = NULL;}}
#define SAFE_DELETE(x) {if(x) { delete x; x = NULL;}}


GraphicClass36::GraphicClass36()
{
	_D3D = NULL;
	_camera = NULL;
	_texshader = NULL;
	_model = NULL;

	_hblurshader = NULL;
	_vblurshader = NULL;
	_renderTex = NULL;
	_downSampleTex = NULL;
	_vblurtex = NULL;
	_hblureTex = NULL;
	_upsampleTex = NULL;
	_smallwindo = NULL;
	_fullwindow = NULL;
}


GraphicClass36::~GraphicClass36()
{
}



bool GraphicClass36::Initialize(int sw, int sh, HWND hwnd)
{
	bool result;
	int downsampleW, downsampleH;

	downsampleW = sw / 2;
	downsampleH = sh / 2;

	NEW_CLASS(_D3D, D3dclass);
	result = _D3D->Initialize(sw, sh, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	V_RETURN(result, L"Not D3d");

	NEW_CLASS(_camera, CameraClass);

	_camera->SetPosition(0, 0, -10);

	NEW_CLASS(_model, ModelClass);

	V_RETURN(_model->Initailize(GETDEVICE, "Cube2.txt", L"Texture/firetex.gif"), L"Not floor");

	NEW_CLASS(_texshader, TextureShaderClass);
	V_RETURN(_texshader->Initialize(GETDEVICE, hwnd), L"Not Texshader");

	NEW_CLASS(_hblurshader, HorizontalBlurShaderClass);
	V_RETURN(_hblurshader->Initialize(GETDEVICE, hwnd), L"hblur not");

	NEW_CLASS(_vblurshader, VerticalBlurShaderClass);
	V_RETURN(_vblurshader->Initialize(GETDEVICE, hwnd), L"vblur not");

	NEW_CLASS(_renderTex, RenderTextureClass);
	V_RETURN(_renderTex->Initialize(GETDEVICE, downsampleW, downsampleH), L"Not rendertex");

	NEW_CLASS(_downSampleTex, RenderTextureClass);
	V_RETURN(_downSampleTex->Initialize(GETDEVICE, downsampleW, downsampleH), L"Not rendertex");

	NEW_CLASS(_hblureTex, RenderTextureClass);
	V_RETURN(_hblureTex->Initialize(GETDEVICE, downsampleW, downsampleH), L"Not rendertex");

	NEW_CLASS(_vblurtex, RenderTextureClass);
	V_RETURN(_vblurtex->Initialize(GETDEVICE, downsampleW, downsampleH), L"Not rendertex");

	NEW_CLASS(_upsampleTex, RenderTextureClass);
	V_RETURN(_upsampleTex->Initialize(GETDEVICE, downsampleW, downsampleH), L"Not rendertex");

	NEW_CLASS(_smallwindo, OrthoWindowClass);
	V_RETURN(_smallwindo->Initialize(GETDEVICE, downsampleW, downsampleH), L"Not small");

	NEW_CLASS(_fullwindow, OrthoWindowClass);
	V_RETURN(_fullwindow->Initialize(GETDEVICE, downsampleW, downsampleH), L"Not small");

	return true;
}

void GraphicClass36::ShutDown()
{
	SHUTDOWN_OBJ(_fullwindow);
	SHUTDOWN_OBJ(_smallwindo);

	SHUTDOWN_OBJ(_upsampleTex);
	SHUTDOWN_OBJ(_vblurshader);
	SHUTDOWN_OBJ(_hblureTex);
	SHUTDOWN_OBJ(_downSampleTex);
	SHUTDOWN_OBJ(_renderTex);


	SHUTDOWN_OBJ(_vblurshader);
	SHUTDOWN_OBJ(_hblurshader);
	SHUTDOWN_OBJ(_texshader);
	SHUTDOWN_OBJ(_model);
	SAFE_DELETE(_camera);
	SHUTDOWN_OBJ(_D3D);

}

bool GraphicClass36::Frame(D3DXVECTOR3 rotation)
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

bool GraphicClass36::Render(float rot)
{
	bool result;
	/*
	D3DXMATRIX world, view, proj, translate;
	
	D3DXVECTOR3 cameraPos, modelPos;
	double angle;
	float rotation;

	_D3D->BeginScene(0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	_D3D->GetProjectionMatrix(proj);

	_billboardModel->Render(_D3D->GetDeviceContext());
	IS_V(_textureShader->Render(_D3D->GetDeviceContext(), _billboardModel->GetIndexCount(), world, view, proj, _billboardModel->GetTexture()));



	_D3D->EndScene();
	*/

	IS_V(RenderSceneToTexture(rot));

	IS_V(DownSampleTexture());

	IS_V(RenderHorizontalBlurToTexture());

	IS_V(RenderVerticalBlureToTexture());

	IS_V(UpSampleTexture());

	IS_V(Render2DTextureScene());

	return true;
}

bool GraphicClass36::RenderSceneToTexture(float rot)
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
	IS_V(_texshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture()));

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass36::DownSampleTexture()
{
	D3DXMATRIX world, view,ortho;
	bool result;

	_downSampleTex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());
	_downSampleTex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);
	
	_D3D->GetOrthoMatrix(ortho);
	
	_D3D->TurnZBufferOff();

	_smallwindo->Render(_D3D->GetDeviceContext());
	IS_V(_texshader->Render(_D3D->GetDeviceContext(), _smallwindo->GetIndexCount(), world, view, ortho, _renderTex->GetShaderResourceView()));

	_D3D->TurnZBufferOn();

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass36::RenderHorizontalBlurToTexture()
{
	D3DXMATRIX world, view, ortho;
	bool result;
	float screensizex;

	screensizex = (float)_hblureTex->GetTextureWidth();

	_hblureTex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());
	_hblureTex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);

	_D3D->GetOrthoMatrix(ortho);

	_D3D->TurnZBufferOff();

	_smallwindo->Render(_D3D->GetDeviceContext());
	IS_V(_hblurshader->Render(_D3D->GetDeviceContext(), _smallwindo->GetIndexCount(), world, view, ortho, _downSampleTex->GetShaderResourceView(),screensizex));

	_D3D->TurnZBufferOn();

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass36::RenderVerticalBlureToTexture()
{
	D3DXMATRIX world, view, ortho;
	bool result;
	float screensizex;

	screensizex = (float)_hblureTex->GetTextureHeight();

	_vblurtex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());
	_vblurtex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);

	_D3D->GetOrthoMatrix(ortho);

	_D3D->TurnZBufferOff();

	_smallwindo->Render(_D3D->GetDeviceContext());
	IS_V(_vblurshader->Render(_D3D->GetDeviceContext(), _smallwindo->GetIndexCount(), world, view, ortho, _hblureTex->GetShaderResourceView(), screensizex));

	_D3D->TurnZBufferOn();

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass36::UpSampleTexture()
{
	D3DXMATRIX world, view, ortho;
	bool result;
	float screensizex;

	screensizex = (float)_hblureTex->GetTextureHeight();

	_upsampleTex->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());
	_upsampleTex->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0, 0, 0, 1);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);

	_D3D->GetOrthoMatrix(ortho);

	_D3D->TurnZBufferOff();

	_fullwindow->Render(_D3D->GetDeviceContext());
	IS_V(_vblurshader->Render(_D3D->GetDeviceContext(), _fullwindow->GetIndexCount(), world, view, ortho, _vblurtex->GetShaderResourceView(), screensizex));

	_D3D->TurnZBufferOn();

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass36::Render2DTextureScene()
{
	D3DXMATRIX world, view, ortho;
	bool result;
	float screensizex;

	_D3D->BeginScene(1, 0, 0, 0);

	_camera->Render();

	_D3D->GetWorldMatrix(world);
	_camera->GetViewMatrix(view);

	_D3D->GetOrthoMatrix(ortho);

	_D3D->TurnZBufferOff();

	_fullwindow->Render(_D3D->GetDeviceContext());
	IS_V(_texshader->Render(_D3D->GetDeviceContext(), _fullwindow->GetIndexCount(), world, view, ortho, _upsampleTex->GetShaderResourceView()));
	

	_D3D->TurnZBufferOn();

	_D3D->EndScene();

	screensizex = (float)_hblureTex->GetTextureHeight();

	_D3D->SetBackBufferRenderTarget();

	return true;
}