#pragma once

#ifndef __GraphicClass_H_
#define __GraphicClass_H_

#include "D3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "BitmapClass.h"

#define __CHAPTER_FIVE__

#include "TextureShaderClass.h"

#include "TextClass.h"

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
	bool Frame(int,int,int,int,float);

private:
	bool Render(float);

private:
	D3dclass* _D3D;

	CameraClass* _Camera;
	ModelClass* _model;
	TextureShaderClass* _shader;

	BitmapClass* _bitmap;
	TextClass *_text;


private:
	//�� ���纯��
	int mX, mY;
};

#endif