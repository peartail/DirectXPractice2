#include "PositionClass.h"
#include "d3dx10math.h"

PositionClass::PositionClass()
{
	_frameTime = 0.f;
	_roationY = 0.f;
	_leftTurnSpeed = 0.f;
	_rightTurnSpeed = 0.f;

	_positionZ = _positionX =  0.f;
	_frontTurnSpeed = 0.f;
	_backTurnSpeed = 0.f;
}

PositionClass::PositionClass(const PositionClass& other)
{

}

PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	_frameTime = time;
	return;
}

void PositionClass::GetRotation(float& y)
{
	y = _roationY;
}

void PositionClass::GetPositionZ(float& z)
{
	z = _positionZ;
}

void PositionClass::GetPositionX(float& x)
{
	x = _positionX;
}


void PositionClass::TurnLeft(bool keydown)
{
	if (keydown)
	{
		_leftTurnSpeed += _frameTime * 0.01f;

		if (_leftTurnSpeed > (_frameTime*0.15f))
		{
			_leftTurnSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_leftTurnSpeed -= _frameTime*0.005f;
		if (_leftTurnSpeed < 0.0f)
		{
			_leftTurnSpeed = 0.0f;
		}
	}

	_roationY -= _leftTurnSpeed;
	if (_roationY < 0.0f)
	{
		_roationY += 360.f;
	}
}

void PositionClass::TurnRight(bool keydown)
{
	if (keydown)
	{
		_rightTurnSpeed += _frameTime * 0.01f;

		if (_rightTurnSpeed > (_frameTime*0.15f))
		{
			_rightTurnSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_rightTurnSpeed -= _frameTime*0.005f;
		if (_rightTurnSpeed < 0.0f)
		{
			_rightTurnSpeed = 0.0f;
		}
	}

	_roationY += _rightTurnSpeed;
	if (_roationY < 0.0f)
	{
		_roationY += 360.f;
	}
}

void PositionClass::TurnFront(bool keydown)
{
	if (keydown)
	{
		_frontTurnSpeed += _frameTime * 0.01f;

		if (_frontTurnSpeed > (_frameTime*0.15f))
		{
			_frontTurnSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_frontTurnSpeed -= _frameTime*0.005f;
		if (_frontTurnSpeed < 0.0f)
		{
			_frontTurnSpeed = 0.0f;
		}
	}
	_positionX += sinf(D3DX_PI*_roationY / 180) * _frontTurnSpeed;
	_positionZ += cosf(D3DX_PI*_roationY / 180) * _frontTurnSpeed;

	//_positionZ += _frontTurnSpeed;

}

void PositionClass::TurnBack(bool keydown)
{
	if (keydown)
	{
		_backTurnSpeed += _frameTime * 0.01f;

		if (_backTurnSpeed > (_frameTime*0.15f))
		{
			_backTurnSpeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_backTurnSpeed -= _frameTime*0.005f;
		if (_backTurnSpeed < 0.0f)
		{
			_backTurnSpeed = 0.0f;
		}
	}

	_positionX -= sinf(D3DX_PI*_roationY / 180) * _backTurnSpeed;
	_positionZ -= cosf(D3DX_PI*_roationY / 180) * _backTurnSpeed;

	//_positionZ -= _backTurnSpeed;

}