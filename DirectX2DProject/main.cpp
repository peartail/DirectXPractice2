#include "SystemClass.h"


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hpreinst, PSTR pSmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	System->ShutDown();
	delete System;
	System = NULL;

	return S_OK;
}