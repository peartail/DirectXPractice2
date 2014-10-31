#include "FrustumClass.h"


FrustumClass::FrustumClass()
{
}

FrustumClass::FrustumClass(const FrustumClass&)
{
}

FrustumClass::~FrustumClass()
{
}

//얘가 프러스텀 행렬의 평면계산이래요. <| <- 요거겠지
void FrustumClass::ConstructFrustum(float screenDepth, D3DXMATRIX proj, D3DXMATRIX view)
{
	float zMinimum, r;
	D3DXMATRIX matrix;

	zMinimum = -proj._43 / proj._33;
	r = screenDepth / (screenDepth - zMinimum);
	proj._33 = r;
	proj._43 = -r * zMinimum;

	D3DXMatrixMultiply(&matrix, &view, &proj);

	_planes[0].a = matrix._14 + matrix._13;
	_planes[0].b = matrix._24 + matrix._23;
	_planes[0].c = matrix._34 + matrix._33;
	_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&_planes[0], &_planes[0]);

	_planes[1].a = matrix._14 + matrix._13;
	_planes[1].b = matrix._24 + matrix._23;
	_planes[1].c = matrix._34 + matrix._33;
	_planes[1].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&_planes[1], &_planes[1]);

	_planes[2].a = matrix._14 + matrix._13;
	_planes[2].b = matrix._24 + matrix._23;
	_planes[2].c = matrix._34 + matrix._33;
	_planes[2].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&_planes[2], &_planes[2]);

	_planes[3].a = matrix._14 + matrix._13;
	_planes[3].b = matrix._24 + matrix._23;
	_planes[3].c = matrix._34 + matrix._33;
	_planes[3].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&_planes[3], &_planes[3]);

	_planes[4].a = matrix._14 + matrix._13;
	_planes[4].b = matrix._24 + matrix._23;
	_planes[4].c = matrix._34 + matrix._33;
	_planes[4].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&_planes[4], &_planes[4]);

	_planes[5].a = matrix._14 + matrix._13;
	_planes[5].b = matrix._24 + matrix._23;
	_planes[5].c = matrix._34 + matrix._33;
	_planes[5].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&_planes[5], &_planes[5]);

}


bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3(x, y, z)) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float rad)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - rad), (yCenter - rad), (zCenter - rad))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter + rad), (yCenter - rad), (zCenter - rad))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - rad), (yCenter + rad), (zCenter - rad))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter + rad), (yCenter + rad), (zCenter - rad))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - rad), (yCenter - rad), (zCenter + rad))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter + rad), (yCenter - rad), (zCenter + rad))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - rad), (yCenter + rad), (zCenter + rad))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter + rad), (yCenter - rad), (zCenter + rad))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float rad)
{
	int i;

	for (i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3(xCenter, yCenter, zCenter)) < -rad)
		{
			return false;
		}
	}
	return true;
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	int i = 0;

	for (i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&_planes[i], &D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}