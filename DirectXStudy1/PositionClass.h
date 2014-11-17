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
	void GetRotation(float&);
	void GetPositionZ(float&);
	void GetPositionX(float&);


	void TurnLeft(bool);
	void TurnRight(bool);
	void TurnFront(bool);
	void TurnBack(bool);

private:
	float _frameTime;
	float _roationY;
	float _positionZ;
	float _positionX;
	float _leftTurnSpeed, _rightTurnSpeed;
	float _frontTurnSpeed, _backTurnSpeed;

};

#endif
