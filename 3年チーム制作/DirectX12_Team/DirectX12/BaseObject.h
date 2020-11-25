#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include<cmath>
enum ObjectType//�I�u�W�F�N�g�̃^�O
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
	virtual void Hit(BaseObject& other) = 0;//�q�b�g�ʒm
	bool StoS_Col(BaseObject& other);//3D���΋������蔻��
	bool BtoB_Col(BaseObject& other);//3D���Δ�
	
	Vector3 RotateX(float x);
	Vector3 RotateY(float y);
	Vector3 RotateZ(float z);
	//Get�@Set
	virtual ObjectType GetType() { return objType; }
	virtual bool GetDeath() { return death; }
	virtual Vector3 GetPosition() { return position; }
	virtual Vector3 GetAngle() { return angle; }
protected:
	ObjectType objType;
	bool death;//����ł��邩
	Vector3 position;
	Vector3 velocity;
	Vector3 angle;
	//���p�̔��a�T�C�Y
	float SphereSize;
	const float PI = 3.1415926535897932384626433832795f;
};
