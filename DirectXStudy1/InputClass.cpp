#include "InputClass.h"

#include <string>
#include <iostream>

InputClass::InputClass()
{
	_directInput = NULL;
	_keyboard = NULL;
	_mouse = NULL;

	_isLeftClicked = false;
}


InputClass::InputClass(const InputClass& other)
{

}

InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinst,HWND hwnd,int screenWidth,int screenHeight)
{
	HRESULT result;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_vmouseX = _mouseX = 0;
	_vmouseY = _mouseY = 0;

	result = DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//result = _keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//result = _mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	if (_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = NULL;
	}

	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = NULL;
	}

	if (_directInput)
	{
		_directInput->Release();
		_directInput = NULL;
	}
}

bool InputClass::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	result = _keyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);

	
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}


void InputClass::ProcessInput()
{
	_mouseX += _mouseState.lX;
	_mouseY += _mouseState.lY; 

	if (_mouseX < 0) {
		_vmouseX = 0;
	}
	else if (_mouseX > _screenWidth)
	{
		_vmouseX = _screenWidth;
	}
	else
	{
		_vmouseX = _mouseX;
	}

	if (_mouseY < 0)
	{
		_vmouseY = 0;
	}
	else if (_mouseY > _screenHeight) { _vmouseY = _screenHeight; }
	else
	{
		_vmouseY = _mouseY;
	}
}


bool InputClass::IsEscapePressed()
{
	if (_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsMouseLeftClick()
{
	
	bool result = false;

	if (_mouseState.rgbButtons[0] != NULL && !_isLeftClicked)
	{
		result = true;
		
		/*std::string data = "\n1 : " + std::to_string(_mouseState.rgbButtons[0]) + " | 2 : " + std::to_string(_mouseState.rgbButtons[1]) + " | 3 : " + std::to_string(_mouseState.rgbButtons[2]) + " | 4 : " + std::to_string(_mouseState.rgbButtons[3]);
		OutputDebugStringA(data.c_str());*/
	}

	if (_mouseState.rgbButtons[0] != NULL)
	{
		_isLeftClicked = true;
	}
	else
	{
		_isLeftClicked = false;
	}
	
	
	return result;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = _vmouseX;
	mouseY = _vmouseY;
}

bool InputClass::IsLeftPressed()
{
	if (_keyboardState[DIK_LEFT] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsRightPressed()
{
	if (_keyboardState[DIK_RIGHT] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsUpPressed()
{
	if (_keyboardState[DIK_UP] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsDownPressed()
{
	if (_keyboardState[DIK_DOWN] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsAPressed()
{
	if (_keyboardState[DIK_A] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsZPressed()
{
	if (_keyboardState[DIK_Z] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsPgUpPressed()
{
	if (_keyboardState[DIK_U] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsPgDownPressed()
{
	if (_keyboardState[DIK_J] & 0x80)
	{
		return true;
	}
	return false;
}

bool InputClass::IsLeftMouseButtonDown()
{
	if (_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}
	return false;
}