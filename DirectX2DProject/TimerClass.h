#pragma once

#ifndef __TIMERCLASS_H__
#define __TIMERCLASS_H__

#include <Windows.h>

class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass&);
	~TimerClass();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 _frequency;
	float _ticksPerMs;
	INT64 _startTime;
	float _frameTime;
};


#endif