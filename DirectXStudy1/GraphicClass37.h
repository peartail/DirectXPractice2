#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100000.0f;
const float SCREEN_NEAR = 1.0f;

#include "D3dclass.h"
#include "CameraClass.h"

//37¿Â
#include "InstanceModelClass.h"
#include "InstanceTextureShaderClass.h"

//38¿Â
//#include "TeselColorShader.h"
//#include "ModelClass.h"

class GraphicClass37
{
public:
	GraphicClass37();
	~GraphicClass37();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation);

private:

	bool Render(float);

private:
	D3dclass* _D3D;
	CameraClass* _camera;

	InstanceModelClass* _model;
	InstanceTextureShaderClass* _shader;

	//ModelClass *_model;
	//TeselColorShader* _shader;
};

