#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "RenderTextureClass.h"

#include "ReflectionShaderClass.h"

class GraphicClass27
{
public:
	GraphicClass27();
	~GraphicClass27();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame();
	bool Render();

private:
	bool RenderToTexture();
	bool RenderScene();

private:
	D3dclass* _D3D;
	CameraClass* _camera;
	ModelClass* _model;
	TextureShaderClass* _texShader;

	RenderTextureClass* _rendertex;
	ModelClass* _floor;

	ReflectionShaderClass* _refshader;
};

