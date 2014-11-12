#include "CameraClass.h"


CameraClass::CameraClass()
{
	_posX = 0.0f;
	_posY = 0.0f;
	_posZ = 0.0f;

	_rotX = 0.0f;
	_rotY = 0.0f;
	_rotZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	_posX = x;
	_posY = y;
	_posZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	_rotX = x;
	_rotY = y;
	_rotZ = z;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(_posX, _posY, _posZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(_rotX, _rotY, _rotZ);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotMatrix;

	// Vector upward(?) ��ġ(���Ⱑ ī�޶� �밡�� �����ΰ�... �ƹ����� normalize ������..
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// ī�޶� ��ġ����?
	position.x = _posX;
	position.y = _posY;
	position.z = _posZ;

	//ī�޶� �Ĵٺ��� �����ΰ�?(���⺤��)
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = _rotX * 0.0174532925f;
	yaw = _rotY * 0.0174532925f;
	roll = _rotZ * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&_viewMatrix, &position, &lookAt, &up);
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMat)
{
	viewMat = _viewMatrix;
}

void CameraClass::RenderReflection(float h)
{
	D3DXVECTOR3 up, pos, lookat;
	float rad;

	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	pos.x = _posX;
	pos.y = _posY + (h + 2.0f);
	pos.z = _posZ;

	rad = _rotY * 0.0174532925f;

	lookat.x = sinf(rad) + _posX;
	lookat.y = pos.y;
	lookat.z = cosf(rad) + _posZ;

	D3DXMatrixLookAtLH(&_reflectionViewmat, &pos, &lookat, &up);


}

D3DXMATRIX CameraClass::GetReflectionViewMatrix()
{
	return _reflectionViewmat;
}