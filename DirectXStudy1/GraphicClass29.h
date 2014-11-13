#pragma once


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "ReflectionShaderClass.h"
#include "WaterShaderClass.h"

#include "RenderTextureClass.h"

class GraphicClass29
{
public:
	GraphicClass29();
	~GraphicClass29();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(float);
	bool Render();

private:
	bool RenderToTexture();
	bool RenderScene();
	bool RenderNormalScene(float);
	bool RenderFadingScene();

private:
	D3dclass* _D3D;
	CameraClass* _camera;
	ModelClass* _groundmodel,*_wallmodel,*_bathmodel,*_watermodel;
	TextureShaderClass* _texShader;

	RenderTextureClass* _rendertex;
	ModelClass* _floor;

	ReflectionShaderClass* _refshader;


};

