#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "RenderTextureClass.h"
#include "TextureShaderClass.h"

#include "GlassShaderClass.h"


class GraphicClass32
{
public:
	GraphicClass32();
	~GraphicClass32();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation);

private:
	bool RenderToTexture(float);
	bool Render(float);

private:
	D3dclass* _D3D;
	CameraClass* _camera;

	ModelClass* _model;
	ModelClass* _windoModel;

	RenderTextureClass* _renderTex;

	TextureShaderClass* _texshader;
	GlassShaderClass* _glassshader;

	float power;
};

