#pragma once

#ifndef __GraphicClass_H_
#define __GraphicClass_H_

#include <windows.h>

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
	bool Render();
};

#endif