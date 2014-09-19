#pragma once

#ifndef __GraphicClass_H_
#define __GraphicClass_H_

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"

#ifdef __CHATER_FOUR__
#include "ColorShaderClass.h"
#elif defined __CHAPTER_FIVE__
#include "TextureShaderClass.h"
#elif defined __CHAPTER_SIX__
#include "LightClass.h"
#include "LightShaderClass.h"
#endif

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicClass
{
public:
	GraphicClass();
	GraphicClass(const GraphicClass&);
	~GraphicClass();

	bool Initialize(int, int, HWND);
	void ShutDown();
	bool Frame();

private:
	bool Render(float);

private:
	D3dclass* _D3D;

	CameraClass* _Camera;
	ModelClass* _model;
#ifdef __CHATER_FOUR__
	ColorShaderClass* _shader;
#elif defined __CHAPTER_FIVE__
	TextureShaderClass* _shader;
#elif defined __CHAPTER_SIX__
	LightShaderClass* _shader;
	LightClass* _light;
#endif
};

#endif