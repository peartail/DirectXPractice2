#include "GraphicClass.h"


#include <string>
#include <iostream>


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

	_2dshader = NULL;
	_bitmap = NULL;
	_text = NULL;

	_modelist = NULL;
	_frustum = NULL;

	_multitexshader = NULL;
	_lightmapshader = NULL;
	_alphamapshader = NULL;
	_bumpmapshader = NULL;
	_specmapshader = NULL;

	_rendertexture = NULL;
	_debugwindow = NULL;
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
	D3DXMATRIX baseViewMatrix;

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
	_Camera->Render();
	_Camera->GetViewMatrix(baseViewMatrix);

	_text = new TextClass;
	if (!_text)
	{
		return false;
	}

	result = _text->Initialize(_D3D->GetDevice(), _D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"CNITTX", L"Error", MB_OK);
		return false;
	}

	_2dshader = new TextureShaderClass;
	if (!_2dshader)
	{
		return false;
	}

	result = _2dshader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Coud not init colorshader", L"Error", MB_OK);
		return false;
	}


	_bitmap = new UIClass;
	if (!_bitmap)
	{
		return false;
	}

	//result = _bitmap->Initialize(_D3D->GetDevice(), screenWidth, screenHeight, L"Texture/rocks_NM_height.dds", 256, 256);
	result = _bitmap->Initialize(_D3D->GetDevice(), screenWidth, screenHeight, L"Texture/128.png", 12, 12,baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Coud not init bitmap obj", L"ERror", MB_OK);
	}

	//////////////////////////////////////////////여기까지가 2d 


	//_Camera->SetRotation(10.0f, 0.0f, 0.0f);
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

#endif

	/*
	//1개의 텍스쳐를 사용하는 스페큘러, 디퓨즈 앰비언트 다들어가는 쉐이더
	result = _model->Initailize(_D3D->GetDevice(), "Cube.txt",L"Texture/rocks_NM_height.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not init model", L"Error", MB_OK);
		return false;
	}

	_shader = new LightShaderClass;



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

	_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light->SetDirection(0.f, 0.0f, 1.0f);
	_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light->SetSpecularPower(32.f);

	*/

	/*
	//멀티 텍스쳐링
	result = _model->Initailize(_D3D->GetDevice(), "Cube.txt", L"Texture/Column.jpg", L"Texture/rocks_NM_height.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Not model obj", L"Err", MB_OK);
		return false;
	}

	_multitexshader = new MultiTextureShaderClass;
	if (!_multitexshader)
	{
		return false;
	}

	result = _multitexshader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Not multitex", L"Err", MB_OK);
		return false;
	}
	*/

	/*
	//라이트맵
	result = _model->Initailize(_D3D->GetDevice(), "Cube.txt", L"Texture/block.gif", L"Texture/lightmaptex.gif");
	if (!result)
	{
		MessageBox(hwnd, L"Not model obj", L"Err", MB_OK);
		return false;
	}

	_lightmapshader = new LightmapShaderClass;
	if (!_lightmapshader)
	{
		return false;
	}

	result = _lightmapshader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Not lightmap", L"Err", MB_OK);
		return false;
	}
	*/

/*
//알파매핑
	result = _model->Initailize(_D3D->GetDevice(), "Cube.txt", L"Texture/block.gif", L"Texture/rocks_NM_height.dds", L"Texture/gradiant.gif");
	if (!result)
	{
		MessageBox(hwnd, L"Not model obj", L"Err", MB_OK);
		return false;
	}

	_alphamapshader = new AlphamapShaderClass;
	if (!_alphamapshader)
	{
		return false;
	}

	result = _alphamapshader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Not alpha", L"Err", MB_OK);
		return false;

	}
	*/

/*
//범프매핑
	result = _model->Initailize(_D3D->GetDevice(), "Cube.txt", L"Texture/block.gif", L"Texture/bumpmap.gif");
	if (!result)
	{
		MessageBox(hwnd, L"Not model obj", L"Err", MB_OK);
		return false;
	}

	_bumpmapshader = new BumpmapShaderClass;
	if (!_bumpmapshader)
	{
		return false;
	}

	result = _bumpmapshader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Not bump", L"Err", MB_OK);
		return false;
	}

	_light = new LightClass;
	if (!_light)
	{
		return false;
	}

	_light->SetDiffuseColor(1.0f, 1.f, 1.f, 1.f);
	_light->SetDirection(0.f, 0.f, 1.f);
	*/

result = _model->Initailize(_D3D->GetDevice(), "Cube.txt", L"Texture/stone.gif", L"Texture/bump02.gif", L"Texture/spec02.gif");
	if (!result)
	{
		MessageBox(hwnd, L"Not model obj", L"Err", MB_OK);
		return false;
	}

	_specmapshader = new SpecmapShaderClass;
	if (!_specmapshader)
	{
		return false;
	}

	result = _specmapshader->Initialize(_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Not specmap", L"Err", MB_OK);
		return false;
	}

	_light = new LightClass;
	if (!_light)
	{
		return false;
	}

	_light->SetDiffuseColor(1.0f, 1.f, 1.f, 1.f);
	_light->SetDirection(1.f, 0.f, 1.f);
	_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light->SetSpecularPower(1.f);
	
	_rendertexture = new RenderTextureClass;
	if (!_rendertexture)
	{
		return false;
	}

	result = _rendertexture->Initialize(_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	_debugwindow = new DebugWindowsClass;
	if (!_debugwindow)
	{
		return false;
	}

	result = _debugwindow->Initialize(_D3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Not Debug WindowObj", L"Err", MB_OK);
		return false;
	}

	_modelist = new ModellistClass;
	if (!_modelist)
	{
		return false;
	}

	result = _modelist->Initialize(2500);
	if (!result)
	{
		MessageBox(hwnd, L"N modelist", L"Err", MB_OK);
		return false;
	}

	_frustum = new FrustumClass;
	if (!_frustum)
	{
		return false;
	}

	return true;
}

void GraphicClass::ShutDown()
{

	if (_rendertexture)
	{
		_rendertexture->Shutdown();
		delete _rendertexture;
		_rendertexture = NULL;
	}

	if (_debugwindow)
	{

		_debugwindow->Shutdown();
		delete _debugwindow;
		_debugwindow = NULL;
	}


	if (_specmapshader)
	{
		_specmapshader->Shutdown();
		delete _specmapshader;
		_specmapshader = NULL;
	}

	if (_alphamapshader)
	{
		_alphamapshader->Shutdown();
		delete _alphamapshader;
		_alphamapshader = NULL;
	}

	if (_lightmapshader)
	{
		_lightmapshader->Shutdown();
		delete _lightmapshader;
		_lightmapshader = NULL;
	}
	if (_multitexshader)
	{
		delete _multitexshader;
		_multitexshader = NULL;
	}

	
	if (_frustum)
	{
		delete _frustum;
		_frustum = NULL;
	}

	if (_modelist)
	{
		_modelist->Shutdown();
		delete _modelist;
		_modelist = NULL;
	}

	if (_bitmap)
	{
		_bitmap->Shutdown();
		delete _bitmap;
		_bitmap = NULL;
	}

	if (_2dshader)
	{
		_2dshader->Shutdown();
		delete _2dshader;
		_2dshader = NULL;
	}

	if (_text)
	{
		_text->Shutdown();
		delete _text;
		_text = NULL;
	}

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

bool GraphicClass::Frame(D3DXVECTOR3 rotation,int mouseX, int mouseY, int fps, int cpu, float ftime)
{
	bool result;

	static float rot = 0.0f;

	_Camera->SetRotation(0.0f, rotation.y, 0.0f);
	_Camera->SetPosition(_Camera->GetPosition().x, _Camera->GetPosition().y, rotation.z);

	rot += (float)D3DX_PI * 0.005f;
	if (rot > 360.0f)
	{
		rot -= 360.0f;
	}

	mX = mouseX;
	mY = mouseY;
	result = _text->SetMousePosition(mouseX, mouseY, _D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	result = _text->SetFps(fps, _D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = _text->SetCpu(cpu, _D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = Render(rot);

	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicClass::RenderToTexture()
{
	bool result;

	_rendertexture->SetRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView());

	_rendertexture->ClearRenderTarget(_D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0.0f, 0.f, 1.f, 1.f);

	result = RenderScene(180);
	if (!result)
	{
		return false;
	}

	_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicClass::RenderScene(float rotation)
{
	D3DXMATRIX world, view, proj;
	int modelCnt, renderCnt = 0, index;
	bool result;
	D3DXVECTOR4 color;
	bool renderModel;
	float posx, posy, posz, rad;

	D3DXVECTOR3 pos = _Camera->GetPosition();

	//pos.x--;

	_Camera->SetRotation(0, rotation, 0);

	//_Camera->SetPosition(pos.x, pos.y, pos.z);

	_Camera->Render();

	_Camera->GetViewMatrix(view);
	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	/*std::string data = "\n1 : " + std::to_string((int)view._11) + ":" + std::to_string((int)view._21) + ":" + std::to_string((int)view._31) + ":" + std::to_string((int)view._41);
	OutputDebugStringA(data.c_str());
	data = "\n1 : " + std::to_string((int)view._12) + ":" + std::to_string((int)view._22) + ":" + std::to_string((int)view._32) + ":" + std::to_string((int)view._42);
	OutputDebugStringA(data.c_str());
	data = "\n1 : " + std::to_string((int)view._13) + ":" + std::to_string((int)view._23) + ":" + std::to_string((int)view._33) + ":" + std::to_string((int)view._43);
	OutputDebugStringA(data.c_str());
	data = "\n1 : " + std::to_string((int)view._14) + ":" + std::to_string((int)view._24) + ":" + std::to_string((int)view._34) + ":" + std::to_string((int)view._44);
	OutputDebugStringA(data.c_str());*/

	_frustum->ConstructFrustum(SCREEN_DEPTH, proj, view);

	modelCnt = _modelist->GetModelCount();

	renderCnt = 0;

	for (index = 0; index < modelCnt; index++)
	{
		_modelist->GetData(index, posx, posy, posz, color);

		rad = 1.0f;

		renderModel = _frustum->CheckSphere(posx, posy, posz, rad);

		if (renderModel)
		{
			//D3DXMatrixTranslation(&world, posx, posy, posz);

			_model->Render(_D3D->GetDeviceContext());
			//_shader->RotationYawPitchRoll(rotation, rotation, rotation);
			//_shader->TranslationMatrix(posx, posy, posz);
			//라이트 다엮은거
			//result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture(), _light->GetDirection(), _light->GetAmbientColor(), color, _Camera->GetPosition(), _light->GetSpecularColor(), _light->GetSpecularPower());
			//멀티텍스쳐
			//result = _multitexshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr());
			//라이트맵
			//result = _lightmapshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr());

			//알파맵
			//result = _alphamapshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr());

			//_bumpmapshader->RotationYawPitchRoll(rotation, rotation, rotation);
			//_bumpmapshader->TranslationMatrix(posx, posy, posz);
			//result = _bumpmapshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr(), _light->GetDirection(), _light->GetDiffuseColor());

			_specmapshader->TranslationMatrix(posx, posy, posz);
			result = _specmapshader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTextureArr(), _light->GetDirection(), _light->GetDiffuseColor(), _Camera->GetPosition(), _light->GetSpecularColor(), _light->GetSpecularPower());

			if (!result)
			{
				return false;
			}

			_D3D->GetWorldMatrix(world);

			renderCnt++;
		}
	}

	//_model->Render(_D3D->GetDeviceContext());

#ifdef __CHATER_FOUR__
	result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj);
#elif defined __CHAPTER_FIVE__
	result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture());
#elif defined __CHAPTER_SIX__
	result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture(), _light->GetDirection(), _light->GetDiffuseColor());
#else 

	//result = _shader->Render(_D3D->GetDeviceContext(), _model->GetIndexCount(), world, view, proj, _model->GetTexture(), _light->GetDirection(), _light->GetAmbientColor(), _light->GetDiffuseColor(), _Camera->GetPosition(), _light->GetSpecularColor(), _light->GetSpecularPower());
#endif
	return true;
}

bool GraphicClass::Render(float rotation)
{
	D3DXMATRIX world, view, proj, ortho;
	bool result;

	int modelCnt, renderCnt = 0, index;
	D3DXVECTOR4 color;
	bool renderModel;
	float posx, posy, posz, rad;

	result = RenderToTexture();
	if (!result)
	{
		return false;
	}

	_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	result = RenderScene();
	if (!result)
	{
		return false;
	}

	_Camera->GetViewMatrix(view);
	_D3D->GetWorldMatrix(world);
	_D3D->GetProjectionMatrix(proj);

	_D3D->GetOrthoMatrix(ortho);
	/*
	D3DXVECTOR3 rot= _Camera->GetRotation();

	//rot.x++;
	rot.z++;

	_Camera->SetRotation(rot.x, rot.y, rot.z);
	*/
	

	result = _text->SetRenderCount(renderCnt, _D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	_D3D->TurnZBufferOff();

	result = _debugwindow->Render(_D3D->GetDeviceContext(), 600, 300);
	if (!result)
	{
		return false;
	}

	result = _2dshader->Render(_D3D->GetDeviceContext(), _debugwindow->GetIndexCount(), world, _bitmap->GetBaseViewMatrix(), ortho, _rendertexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}
	
	result = _bitmap->Render(_D3D->GetDeviceContext(), mX, mY);
	if (!result)
	{
		return false;
	}
	result = _2dshader->Render(_D3D->GetDeviceContext(), _bitmap->GetIndexCount(), world, _bitmap->GetBaseViewMatrix(), ortho, _bitmap->GetTexture());

	_D3D->TurnOnAlphaBlending();

	
	result = _text->Render(_D3D->GetDeviceContext(), world, ortho);
	if (!result)
	{
		return false;
	}

	_D3D->TurnOffAlphaBlending();
	_D3D->TurnZBufferOn();



	//D3DXMatrixRotationY(&world, rotation);
	//D3DXMatrixRotationYawPitchRoll(&world, rotation, rotation, rotation);

	



	_D3D->EndScene();
	return true;
}