#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightClass.h"

#include "RenderTextureClass.h"
#include "DepthShaderClass.h"
#include "MultiShadowShaderClass.h"

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_WEIGHT = 1024;

class GraphicClass41
{
public:
	GraphicClass41();
	~GraphicClass41();
	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation, float, float, float, float, float, float);

private:
	bool RenderSceneToTexture();
	bool RenderSceneToTexture2();
	bool Render();



private:
	D3dclass* _D3D;
	CameraClass* _camera;

	RenderTextureClass *_renderTex;
	

	ModelClass *_cubeModel, *_groundModel, *_spehereModel;
	LightClass *_light;

	DepthShaderClass* _depthshader;
	MultiShadowShaderClass* _shadowshader;

	RenderTextureClass* _renderTex2;
	LightClass* _light2;
};

