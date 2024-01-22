#pragma once

#include "Core/Core.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera& rhs) = default;
	Camera& operator=(const Camera& rhs) = default;
	~Camera();

	// Position
	XMVECTOR GetPos()const;
	XMFLOAT3 GetPos_f()const;
	void SetPos(float x, float y, float z);
	void SetPos(const XMFLOAT3& v);

	// CameraCoord
	XMVECTOR GetRight()const;
	XMVECTOR GetLook()const;
	XMVECTOR GetUp()const;
	XMFLOAT3 GetRight_f()const;
	XMFLOAT3 GetLook_f()const;
	XMFLOAT3 GetUp_f()const;

	// Frustum
	float GetFrustumN()const;
	float GetFrustumF()const;
	float GetFrustumAspect()const;
	float GetFrustumFovY()const;
	float GetFrustumFovX()const;
	float GetFrustumNearWidth()const;
	float GetFrustumNearHeight()const;
	float GetFrustumFarWidth()const;
	float GetFrustumFarHeight()const;

	// Setter
	void SetFrustum(float fovY, float aspect, float n, float f);
	void SetCamera(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR up);
	void SetCamera(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& worldUp);
	
	// Matrix
	XMMATRIX GetViewMatrix()const;
	XMFLOAT4X4 GetViewMatrix_4x4()const;
	XMMATRIX GetProjectionMatrix()const;
	XMFLOAT4X4 GetProjectionMatrix_4x4()const;

	// Camera Move
	void Strafe(float delta);
	void Walk(float delta);
	// Camera Rotation
	void RotatePitch(float angle);
	void RotateYaw(float angle);
	void RotateRoll(float angle);
	void RotateWorldY(float angle);

	void UpdateProjectionMatrix();
	void UpdateViewMatrix();

private:
	// Camera
	XMFLOAT3 _Position;
	XMFLOAT3 _Right;
	XMFLOAT3 _Look;
	XMFLOAT3 _Up;
	
	// Frustum
	float _NearZ;
	float _FarZ;
	float _Aspect;
	float _FovY;
	float _NearHeight;
	float _FarHeight;

	// Matrix
	XMFLOAT4X4 _ViewMatrix;
	XMFLOAT4X4 _ProjectionMatrix;

	bool _bViewDirty;
};