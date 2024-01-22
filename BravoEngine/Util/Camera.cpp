#include "Camera.h"

Camera::Camera()
	: _Position(XMFLOAT3(0,0,0)), _Right(XMFLOAT3(1,0,0)), _Look(XMFLOAT3(0,0,1)), _Up(XMFLOAT3(0,1,0)),
	_NearZ(1.0f), _FarZ(2.0f), _Aspect(1.0f), _FovY(90.0f), _NearHeight(2.0f), _FarHeight(4.0f)
{
	_bViewDirty = true;
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

Camera::~Camera()
{

}

XMVECTOR Camera::GetPos() const
{
	return XMLoadFloat3(&_Position);
}

XMFLOAT3 Camera::GetPos_f() const
{
	return _Position;
}

void Camera::SetPos(float x, float y, float z)
{
	_Position = XMFLOAT3(x, y, z);
	_bViewDirty = true;
}

void Camera::SetPos(const XMFLOAT3& v)
{
	_Position = v;
	_bViewDirty = true;
}

XMVECTOR Camera::GetRight() const
{
	return XMLoadFloat3(&_Right);
}

XMVECTOR Camera::GetLook() const
{
	return XMLoadFloat3(&_Look);
}

XMVECTOR Camera::GetUp() const
{
	return XMLoadFloat3(&_Up);
}

XMFLOAT3 Camera::GetRight_f() const
{
	return _Right;
}

XMFLOAT3 Camera::GetLook_f() const
{
	return _Look;
}

XMFLOAT3 Camera::GetUp_f() const
{
	return _Up;
}

float Camera::GetFrustumN() const
{
	return _NearZ;
}

float Camera::GetFrustumF() const
{
	return _FarZ;
}

float Camera::GetFrustumAspect() const
{
	return _Aspect;
}

float Camera::GetFrustumFovY() const
{
	return _FovY;
}

float Camera::GetFrustumFovX() const
{
	float halfWidth = GetFrustumNearWidth();
	return 2.0f * atan(halfWidth / _NearZ);
}

float Camera::GetFrustumNearWidth() const
{
	return _Aspect * _NearHeight;
}

float Camera::GetFrustumNearHeight() const
{
	return _NearHeight;
}

float Camera::GetFrustumFarWidth() const
{
	return _Aspect * _FarHeight;
}

float Camera::GetFrustumFarHeight() const
{
	return _FarHeight;
}

void Camera::SetFrustum(float fovY, float aspect, float n, float f)
{
	_FovY = fovY;
	_Aspect = aspect;
	_NearZ = n;
	_FarZ = f;

	float factor = 2.0f * tanf(0.5f * fovY);
	_NearHeight = factor * n;
	_FarHeight = factor * f;

	UpdateProjectionMatrix();
}

void Camera::SetCamera(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(target - pos);
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&_Right, R);
	XMStoreFloat3(&_Look, L);
	XMStoreFloat3(&_Up, U);

	_bViewDirty = true;
}

void Camera::SetCamera(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	SetCamera(P, T, U);
}

XMMATRIX Camera::GetViewMatrix() const
{
	return XMLoadFloat4x4(&_ViewMatrix);
}

XMFLOAT4X4 Camera::GetViewMatrix_4x4() const
{
	return _ViewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMLoadFloat4x4(&_ProjectionMatrix);
}

XMFLOAT4X4 Camera::GetProjectionMatrix_4x4() const
{
	return _ProjectionMatrix;
}

void Camera::Strafe(float delta)
{
	XMVECTOR d = XMVectorReplicate(delta);
	XMVECTOR R = XMLoadFloat3(&_Right);
	XMVECTOR P = XMLoadFloat3(&_Position);

	XMStoreFloat3(&_Position, XMVectorMultiplyAdd(d, R, P));
	_bViewDirty = true;
}

void Camera::Walk(float delta)
{
	XMVECTOR d = XMVectorReplicate(delta);
	XMVECTOR L = XMLoadFloat3(&_Look);
	XMVECTOR P = XMLoadFloat3(&_Position);

	XMStoreFloat3(&_Position, XMVectorMultiplyAdd(d, L, P));
	_bViewDirty = true;
}

void Camera::RotatePitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_Right), angle);
	
	XMStoreFloat3(&_Look, XMVector3TransformNormal(XMLoadFloat3(&_Look), R));
	XMStoreFloat3(&_Up, XMVector3TransformNormal(XMLoadFloat3(&_Up), R));

	_bViewDirty = true;
}

void Camera::RotateYaw(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_Up), angle);

	XMStoreFloat3(&_Look, XMVector3TransformNormal(XMLoadFloat3(&_Look), R));
	XMStoreFloat3(&_Right, XMVector3TransformNormal(XMLoadFloat3(&_Right), R));

	_bViewDirty = true;
}

void Camera::RotateRoll(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&_Look), angle);

	XMStoreFloat3(&_Up, XMVector3TransformNormal(XMLoadFloat3(&_Up), R));
	XMStoreFloat3(&_Right, XMVector3TransformNormal(XMLoadFloat3(&_Right), R));

	_bViewDirty = true;
}

// What if Up vector is not worldY?
void Camera::RotateWorldY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&_Right, XMVector3TransformNormal(XMLoadFloat3(&_Right), R));
	XMStoreFloat3(&_Up, XMVector3TransformNormal(XMLoadFloat3(&_Up), R));
	XMStoreFloat3(&_Look, XMVector3TransformNormal(XMLoadFloat3(&_Look), R));

	_bViewDirty = true;
}

void Camera::UpdateProjectionMatrix()
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(_FovY, _Aspect, _NearZ, _FarZ);
	XMStoreFloat4x4(&_ProjectionMatrix, P);
}

void Camera::UpdateViewMatrix()
{
	if (_bViewDirty == true)
	{
		XMVECTOR R = XMLoadFloat3(&_Right);
		XMVECTOR L = XMLoadFloat3(&_Look);
		XMVECTOR U = XMLoadFloat3(&_Up);
		XMVECTOR P = XMLoadFloat3(&_Position);

		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));
		R = XMVector3Cross(U, L);
		
		XMStoreFloat3(&_Right, R);
		XMStoreFloat3(&_Look, L);
		XMStoreFloat3(&_Up, U);

		_ViewMatrix(0, 0) = _Right.x;
		_ViewMatrix(1, 0) = _Right.y;
		_ViewMatrix(2, 0) = _Right.z;
		
		_ViewMatrix(0, 1) = _Up.x;
		_ViewMatrix(1, 1) = _Up.y;
		_ViewMatrix(2, 1) = _Up.z;

		_ViewMatrix(0, 2) = _Look.x;
		_ViewMatrix(1, 2) = _Look.y;
		_ViewMatrix(2, 2) = _Look.z;

		_ViewMatrix(3, 0) = -XMVectorGetX(XMVector3Dot(P, R));
		_ViewMatrix(3, 1) = -XMVectorGetX(XMVector3Dot(P, U));
		_ViewMatrix(3, 2) = -XMVectorGetX(XMVector3Dot(P, L));

		_ViewMatrix(0, 3) = 0.0f;
		_ViewMatrix(1, 3) = 0.0f;
		_ViewMatrix(2, 3) = 0.0f;
		_ViewMatrix(3, 3) = 1.0f;

		_bViewDirty = false;
	}
}
