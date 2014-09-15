#pragma once

#ifndef __CAMERACLASS_H_
#define __CAMERACLASS_H_

#include <D3DX10math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	float _posX, _posY, _posZ;
	float _rotX, _rotY, _rotZ;
	D3DXMATRIX _viewMatrix;
};

#endif