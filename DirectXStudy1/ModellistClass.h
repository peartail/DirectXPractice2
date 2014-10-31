#pragma once

#ifndef __MODELLISTCLASS_H__
#define __MODELLISTCLASS_H__

#include <D3DX10math.h>
#include <stdlib.h>
#include <time.h>



class ModellistClass
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float posx, posy, posz;
	};
public:
	ModellistClass();
	ModellistClass(const ModellistClass&);
	~ModellistClass();

	bool Initialize(int);
	void Shutdown();

	int GetModelCount();
	void GetData(int, float&, float&, float&, D3DXVECTOR4&);
private:
	int _modelCount;
	ModelInfoType* _modelInfoList;
};

#endif