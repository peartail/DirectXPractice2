#pragma once

#ifndef __CPUCLASS_H__
#define __CPUCLASS_H__

#pragma comment(lib,"pdh.lib")

#include <Pdh.h>

class CpuClass
{
public:
	CpuClass();
	CpuClass(const CpuClass&);
	~CpuClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool _canReadCpu;
	HQUERY _queryHandle;
	HCOUNTER _counterHandle;
	unsigned long _lastSampleTime;
	long _cpuUsage;
};



#endif