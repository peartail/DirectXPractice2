#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"

#include "FireShaderClass.h"

class GraphicsClass33
{
public:
	GraphicsClass33();
	~GraphicsClass33();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation);

private:
	
	bool Render(float);

private:
	D3dclass* _D3D;
	CameraClass* _camera;

	ModelClass* _model;

	FireShaderClass* _fireShader;

	ModelClass *_floorModel, *_billboardModel;
};

