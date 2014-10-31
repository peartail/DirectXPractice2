#include "TimerClass.h"


TimerClass::TimerClass()
{
}

TimerClass::TimerClass(const TimerClass& other)
{
}

TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	if (_frequency == 0)
	{
		return false;
	}

	_ticksPerMs = (float)(_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 currentTime;
	float timeDiff;

	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDiff = (float)(currentTime - _startTime);

	_frameTime = timeDiff / _ticksPerMs;

	_startTime = currentTime;

}


float TimerClass::GetTime()
{
	return _frameTime;
}