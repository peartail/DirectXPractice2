#pragma once

#ifndef __LIGHTCLASS_H_
#define __LIGHTCLASS_H_

#include <D3DX10math.h>


class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	void SetPosition(float, float, float);
	//40
	void SetLookAt(float, float, float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetSpecularColor();
	float GetSpecularPower();

	D3DXVECTOR4 GetPosition();

	void GenerateViewMatrix();
	void GenerateProjcetionMatrix(float, float);

	void GetViewMatrix(D3DXMATRIX&);
	void GetProjMatrix(D3DXMATRIX&);
private:
	D3DXVECTOR4 _ambientColor;
	D3DXVECTOR4 _diffuseColor;
	D3DXVECTOR3 _direction;
	D3DXVECTOR4 _specularColor;
	float _specularPower;
	
	D3DXVECTOR4 _position;

	D3DXVECTOR3 _lookat;
	D3DXMATRIX _viewmat;
	D3DXMATRIX _projmat;
};


#endif 