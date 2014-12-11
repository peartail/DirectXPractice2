#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 1.0f;

#include "D3dclass.h"
#include "CameraClass.h"

#include "ParticleShaderClass.h"
#include "ParticleSystemClass.h"

class GraphicClass39
{
public:
	GraphicClass39();
	~GraphicClass39();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame(D3DXVECTOR3 rotation,int ftime);

private:

	bool Render(float);

private:
	D3dclass* _D3D;
	CameraClass* _camera;

	ParticleShaderClass* _particleShader;
	ParticleSystemClass* _particleSystem;
};

