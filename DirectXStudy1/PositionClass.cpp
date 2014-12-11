#include "PositionClass.h"
#include "d3dx10math.h"

PositionClass::PositionClass()
{
	_frameTime = 0.f;
	
	_posx = _posy = _posz = 0;
	_rotx = _roty = _rotz = 0;

	_forwardspeed = _backwardspeed = _upwardspeed = _downwardspeed = _leftturnspeed = _rightturnspeed = 0;
	_lookdownspeed = _lookdownspeed = 0;
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

void PositionClass::SetPosition(float x, float y, float z)
{
	_posx = x;
	_posy = y;
	_posz = z;
}

void PositionClass::SetRotation(float x, float y, float z)
{
	_rotx = x;
	_roty = y;
	_rotz = z;
}

void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = _posx;
	y = _posy;
	z = _posz;
}

void PositionClass::GetRotation(float& x, float& y, float& z)
{
	x = _rotx;
	y = _roty;
	z = _rotz;
}

void PositionClass::MoveForward(bool keydown)
{
	if (keydown)
	{
		_forwardspeed += _frameTime * 0.01f;

		if (_forwardspeed > (_frameTime*0.15f))
		{
			_forwardspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_forwardspeed -= _frameTime*0.005f;
		if (_forwardspeed < 0.0f)
		{
			_forwardspeed = 0.0f;
		}
	}

	float radians = _roty * 0.0174532925f;

	_posx += sinf(radians) * _forwardspeed;
	_posz += cosf(radians) * _forwardspeed;
}

void PositionClass::MoveBackWard(bool keydown)
{
	if (keydown)
	{
		_backwardspeed += _frameTime * 0.01f;

		if (_backwardspeed > (_frameTime*0.15f))
		{
			_backwardspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_backwardspeed -= _frameTime*0.005f;
		if (_backwardspeed < 0.0f)
		{
			_backwardspeed = 0.0f;
		}
	}

	float radians = _roty * 0.0174532925f;

	_posx -= sinf(radians) * _backwardspeed;
	_posz -= cosf(radians) * _backwardspeed;
}

void PositionClass::MoveUpward(bool keydown)
{
	if (keydown)
	{
		_upwardspeed += _frameTime * 0.01f;

		if (_upwardspeed > (_frameTime*0.15f))
		{
			_upwardspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_upwardspeed -= _frameTime*0.005f;
		if (_upwardspeed < 0.0f)
		{
			_upwardspeed = 0.0f;
		}
	}

	_posy += _upwardspeed;
}

void PositionClass::MoveDownward(bool keydown)
{
	if (keydown)
	{
		_downwardspeed += _frameTime * 0.01f;

		if (_downwardspeed > (_frameTime*0.15f))
		{
			_downwardspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_downwardspeed -= _frameTime*0.005f;
		if (_downwardspeed < 0.0f)
		{
			_downwardspeed = 0.0f;
		}
	}

	_posy -= _downwardspeed;
}

void PositionClass::TurnLeft(bool keydown)
{
	if (keydown)
	{
		_leftturnspeed += _frameTime * 0.01f;

		if (_leftturnspeed > (_frameTime*0.15f))
		{
			_leftturnspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_leftturnspeed -= _frameTime*0.005f;
		if (_leftturnspeed < 0.0f)
		{
			_leftturnspeed = 0.0f;
		}
	}

	_roty -= _leftturnspeed;
	if (_roty < 0.0f)
	{
		_roty += 360.f;
	}
}

void PositionClass::TurnRight(bool keydown)
{
	if (keydown)
	{
		_rightturnspeed += _frameTime * 0.01f;

		if (_rightturnspeed > (_frameTime*0.15f))
		{
			_rightturnspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_rightturnspeed -= _frameTime*0.005f;
		if (_rightturnspeed < 0.0f)
		{
			_rightturnspeed = 0.0f;
		}
	}

	_roty += _rightturnspeed;
	if (_roty > 360.f)
	{
		_roty -= 360.f;
	}
}

void PositionClass::LookUpward(bool keydown)
{
	if (keydown)
	{
		_lookupspeed += _frameTime * 0.01f;

		if (_lookupspeed > (_frameTime*0.15f))
		{
			_lookupspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_lookupspeed -= _frameTime*0.005f;
		if (_lookupspeed < 0.0f)
		{
			_lookupspeed = 0.0f;
		}
	}

	_rotx -= _lookupspeed;
	if (_rotx > 90.0f)
	{
		_rotx = 90.f;
	}
}

void PositionClass::LookDownward(bool keydown)
{
	if (keydown)
	{
		_lookdownspeed += _frameTime * 0.01f;

		if (_lookdownspeed > (_frameTime*0.15f))
		{
			_lookdownspeed = _frameTime * 0.15f;
		}
	}
	else
	{
		_lookdownspeed -= _frameTime*0.005f;
		if (_lookdownspeed < 0.0f)
		{
			_lookdownspeed = 0.0f;
		}
	}

	_rotx += _lookdownspeed;
	if (_rotx < -90.0f)
	{
		_rotx = -90.f;
	}
}