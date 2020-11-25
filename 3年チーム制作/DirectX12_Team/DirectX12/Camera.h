#pragma once
#include"Vector3.h"
#include"Matrix4.h"
#include<wrl.h>

using namespace Microsoft::WRL;
class Camera
{
public:
	void Init();
	//カメラ用機能
	static const Vector3& GetEye() { return eye; }//視点座標
	static void SetEye(Vector3 eye);
	static const Vector3& GetTarget() { return target; }//注視点座標
	static void SetTarget(Vector3 target);
	static void CameraMoveVector(Vector3 move);//全て動く
	static void CameraMoveEyeVector(Vector3 move);//視点固定

	static void UpdateViewMatrix();

	//カメラ
	static Matrix4 matView;
	static Vector3 eye;
	static Vector3 target;
	static Vector3 up;
private:
};

