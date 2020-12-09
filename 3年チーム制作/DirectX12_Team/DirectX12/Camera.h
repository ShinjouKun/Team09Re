#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include<wrl.h>

using namespace Microsoft::WRL;
class Camera
{
public:
	void Init();
	//�J�����p�@�\
	static const Vector3& GetEye() { return eye; }//���_���W
	static void SetEye(Vector3 eye);
	static const Vector3& GetTarget() { return target; }//�����_���W
	static void SetTarget(Vector3 target);
	static void CameraMoveVector(Vector3 move);//�S�ē���
	static void CameraMoveEyeVector(Vector3 move);//���_�Œ�

	static void UpdateViewMatrix();

	//�J����
	static Matrix4 matView;
	static Vector3 eye;
	static Vector3 target;
	static Vector3 up;
private:
};

