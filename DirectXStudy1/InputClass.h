#pragma once

#ifndef __InputClass_H_
#define __InputClass_H_

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE,HWND,int,int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);
	bool IsMouseLeftClick();

	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsAPressed();
	bool IsZPressed();
	bool IsPgUpPressed();
	bool IsPgDownPressed();
	bool IsLeftMouseButtonDown();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	unsigned char _keyboardState[256];
	DIMOUSESTATE _mouseState;

	int _screenWidth, _screenHeight;
	int _mouseX, _mouseY;

	//내가 걍 만든거
	int _vmouseX, _vmouseY;
	bool _isLeftClicked;
};

#endif