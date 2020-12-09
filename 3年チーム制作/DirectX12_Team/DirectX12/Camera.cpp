#include "Camera.h"
Vector3 Camera::eye = Vector3{ 0.0f, 0.0f, -100.0f };
Vector3 Camera::target = Vector3::zero;
Vector3 Camera::up = Vector3{ 0.0f, 1.0f, 0.0f };
Matrix4 Camera::matView{};
void Camera::Init()
{
	matView = Matrix4::createLookAt(eye, target, up);
}

void Camera::SetEye(Vector3 eye)
{
	Camera::eye = eye;
	UpdateViewMatrix();
}

void Camera::SetTarget(Vector3 target)
{
	Camera::target = target;
	UpdateViewMatrix();
}

void Camera::CameraMoveVector(Vector3 move)
{
	Vector3 eyeMove = GetEye();
	Vector3 targetMove = GetTarget();

	eyeMove.x += move.x;
	eyeMove.y += move.y;
	eyeMove.z += move.z;

	targetMove.x += move.x;
	targetMove.y += move.y;
	targetMove.z += move.z;

	SetEye(eyeMove);
	SetTarget(targetMove);
}

void Camera::CameraMoveEyeVector(Vector3 move)
{
	Vector3 eyeMove = GetEye();

	eyeMove.x += move.x;
	eyeMove.y += move.y;
	eyeMove.z += move.z;

	SetEye(eyeMove);
}


void Camera::UpdateViewMatrix()
{
	matView = Matrix4::createLookAt(eye, target, up);
}
