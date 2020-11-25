#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include<cmath>
enum ObjectType//オブジェクトのタグ
{
	PLAYER,
	ENEMY,
	BULLET
};
class BaseObject
{
public:
	BaseObject();
	virtual~BaseObject();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Rend() = 0;
	virtual void Hit(BaseObject& other) = 0;//ヒット通知
	bool StoS_Col(BaseObject& other);//3D球対球当たり判定
	bool BtoB_Col(BaseObject& other);//3D箱対箱
	
	Vector3 RotateX(float x);
	Vector3 RotateY(float y);
	Vector3 RotateZ(float z);
	//Get　Set
	virtual ObjectType GetType() { return objType; }
	virtual bool GetDeath() { return death; }
	virtual Vector3 GetPosition() { return position; }
	virtual Vector3 GetAngle() { return angle; }
protected:
	ObjectType objType;
	bool death;//死んでいるか
	Vector3 position;
	Vector3 velocity;
	Vector3 angle;
	//球用の半径サイズ
	float SphereSize;
	const float PI = 3.1415926535897932384626433832795f;
};
