#pragma once

#ifndef __FPSCLASS_H__
#define __FPSCLASS_H__

#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <mmsystem.h>

class FpsClass
{
public:
	FpsClass();
	FpsClass(const FpsClass&);
	~FpsClass();

	void Initialize();
	void Frame();
	int GetFps();

private:
	int _fps, _count;
	unsigned long _startTime;
};



#endif