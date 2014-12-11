#pragma once


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100000.0f;
const float SCREEN_NEAR = 1;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "ReflectionShaderClass.h"

#include "WaterShaderClass.h"

#include "RenderTextureClass.h"

#include "RefractionShaderClass.h"

class GraphicClass29
{
public:
	GraphicClass29();
	~GraphicClass29();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation);
	bool Render();

private:
	bool RenderReflectionToTexture();
	bool RenderRefractionToTexture();
	bool RenderScene();
	

private:
	D3dclass* _D3D;
	CameraClass* _camera;
	ModelClass* _groundmodel,*_wallmodel,*_bathmodel,*_watermodel;

	RenderTextureClass* _reftex,*_refractex;

	LightClass* _light;
	LightShaderClass* _lightshader;

	RefractionShaderClass* _refractShader;

	WaterShaderClass* _watershader;

	float _waterh, _waterTrans;
};

