#include "LightClass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{

}

LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColor(float red, float g, float b, float a)
{
	_diffuseColor = D3DXVECTOR4(red, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
	_direction = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return _diffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return _direction;
}