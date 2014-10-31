#include "ModellistClass.h"


ModellistClass::ModellistClass()
{
	_modelInfoList = NULL;
}


ModellistClass::ModellistClass(const ModellistClass& other)
{

}

ModellistClass::~ModellistClass()
{
}


bool ModellistClass::Initialize(int numModels)
{
	int i;
	float r, g, b;

	_modelCount = numModels;

	_modelInfoList = new ModelInfoType[_modelCount];
	if (!_modelInfoList)
	{
		return false;
	}

	srand((unsigned int)time(NULL));

	for (i = 0; i < _modelCount; i++)
	{
		r = (float)rand() / RAND_MAX;
		g = (float)rand() / RAND_MAX;
		b = (float)rand() / RAND_MAX;

		_modelInfoList[i].color = D3DXVECTOR4(r, g, b, 1.0f);

		_modelInfoList[i].posx = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f;
		_modelInfoList[i].posy = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f;
		_modelInfoList[i].posz = (((float)rand() - (float)rand()) / RAND_MAX) * 10.f;
	}
}