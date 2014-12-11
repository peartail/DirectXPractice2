#pragma once

#ifndef __POSITIONCLASS_H__
#define __POSITIONCLASS_H__

#include <math.h>


class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float);
	
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	void GetRotation(float&, float&, float&);

	void TurnLeft(bool);
	void TurnRight(bool);

	void MoveForward(bool);
	void MoveBackWard(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	
	void LookUpward(bool);
	void LookDownward(bool);

private:
	float _frameTime;

	float _posx, _posy, _posz;
	float _rotx, _roty, _rotz;

	float _forwardspeed, _backwardspeed;
	float _upwardspeed, _downwardspeed;
	float _leftturnspeed, _rightturnspeed;
	float _lookupspeed, _lookdownspeed;
	

};

#endif
