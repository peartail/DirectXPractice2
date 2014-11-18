#pragma once

#ifndef __SYSTEMCLASS_H_
#define __SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h"
//#include "GraphicClass.h"
//#include "GraphicClass27.h"
//#include "GraphicClass29.h"
//#include "GraphicClass30.h"
//#include "GraphicClass32.h"
//#include "GraphicsClass33.h"
//#include "GraphicClass35.h"
#include "GraphicClass36.h"

#include "FpsClass.h"
#include "CpuClass.h"
#include "TimerClass.h"
#include "PositionClass.h"

#include "SoundClass.h"

//#define GraphicClass GraphicClass27
//#define GraphicClass GraphicClass29
//#define GraphicClass GraphicClass30
//#define GraphicClass GraphicClass32
//#define GraphicClass GraphicsClass33
//#define GraphicClass GraphicClass35
#define GraphicClass GraphicClass36

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void ShutDown();
	void Run();

	LRESULT CALLBACK MEssageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicClass* m_Graphics;

	FpsClass* _fps;
	CpuClass* _cpu;
	TimerClass* _timer;

	PositionClass* _position;

	SoundClass* _sound;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = NULL;

#endif