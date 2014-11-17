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

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float g, float b, float a)
{
	_diffuseColor = D3DXVECTOR4(red, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
	_direction = D3DXVECTOR3(x, y, z);
}

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
	_specularColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetSpecularPower(float p)
{
	_specularPower = p;
}

void LightClass::SetPosition(float x, float y, float z)
{
	_position = D3DXVECTOR4(x, y, z, 1);
	
}


D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return _ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return _diffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return _direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return _specularColor;
}

float LightClass::GetSpecularPower()
{
	return _specularPower;
}

D3DXVECTOR4 LightClass::GetPosition()
{
	return _position;
}