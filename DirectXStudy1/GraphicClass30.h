#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "LightClass.h"
#include "Light4ShaderClass.h"

class GraphicClass30
{
public:
	GraphicClass30();
	~GraphicClass30();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation);

private:
	bool Render();

private:
	D3dclass* _D3D;
	CameraClass* _camera;
	ModelClass* _model;
	Light4ShaderClass* _lightshader;

	
	LightClass *_light1, *_light2, *_light3, *_light4;
};

