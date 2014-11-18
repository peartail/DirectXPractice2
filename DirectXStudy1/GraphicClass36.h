#pragma once


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"

#include "HorizontalBlurShaderClass.h"
#include "VerticalBlurShaderClass.h"
#include "RenderTextureClass.h"
#include "OrthoWindowClass.h"

class GraphicClass36
{
public:
	GraphicClass36();
	~GraphicClass36();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation);

private:

	bool Render(float);

	bool RenderSceneToTexture(float);
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlureToTexture();
	bool UpSampleTexture();
	bool Render2DTextureScene();

private:
	D3dclass* _D3D;
	CameraClass* _camera;
	ModelClass* _model;
	TextureShaderClass* _texshader;

	HorizontalBlurShaderClass* _hblurshader;
	VerticalBlurShaderClass* _vblurshader;

	RenderTextureClass *_renderTex, *_downSampleTex, *_hblureTex, *_vblurtex, *_upsampleTex;

	OrthoWindowClass* _smallwindo, *_fullwindow;
};

