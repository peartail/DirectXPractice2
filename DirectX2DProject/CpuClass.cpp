#include "CpuClass.h"


CpuClass::CpuClass()
{
}

CpuClass::CpuClass(const CpuClass& other)
{

}

CpuClass::~CpuClass()
{
}


void CpuClass::Initialize()
{
	PDH_STATUS status;

	_canReadCpu = true;

	status = PdhOpenQuery(NULL, 0, &_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}

	status = PdhAddCounter(_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCpu = false;
	}

	_lastSampleTime = GetTickCount();

	_cpuUsage = 0;
}

void CpuClass::Shutdown()
{
	if (_canReadCpu)
	{
		PdhCloseQuery(_queryHandle);
	}
}

void CpuClass::Frame()
{
	PDH_FMT_COUNTERVALUE val;

	if (_canReadCpu)
	{
		if ((_lastSampleTime + 1000) < GetTickCount())
		{
			_lastSampleTime = GetTickCount();

			PdhCollectQueryData(_queryHandle);

			PdhGetFormattedCounterValue(_counterHandle, PDH_FMT_LONG, NULL, &value);

			_cpuUsage = val.longValue;
		}
	}
}


int CpuClass::GetCpuPercentage()
{
	int usage;

	if (_canReadCpu)
	{
		usage = (int)_cpuUsage;

	}
	else
	{
		usage = 0;
	}
	return usage;
}