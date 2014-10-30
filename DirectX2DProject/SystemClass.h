#pragma once

#ifndef __SYSTEMCLASS_H_
#define __SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h"
#include "GraphicClass.h"
#include "SoundClass.h"

#include "FpsClass.h"
#include "CpuClass.h"
#include "TimerClass.h"


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
	SoundClass* _sound;

	FpsClass* _fps;
	CpuClass* _cpu;
	TimerClass* _timer;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = NULL;


#endif