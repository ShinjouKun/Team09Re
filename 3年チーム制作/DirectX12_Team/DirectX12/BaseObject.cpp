#include "BaseObject.h"

BaseObject::BaseObject()
{
}

BaseObject::~BaseObject()
{
}

bool BaseObject::StoS_Col(BaseObject & other)
{
	//半径の中心点がモデルの原点に依存しているので注意せよ
	float length = Vector3((position - other.position)).Length();
	float radSam = SphereSize + other.SphereSize;
	if (length <= radSam)
	{
		return true;
	}
	return false;
}

bool BaseObject::BtoB_Col(BaseObject & other)
{
	float length = Vector3((position - other.position)).Length();

	return false;
}

Vector3 BaseObject::RotateX(float x)
{
	Vector3 v;

	x = x + 45.0f;
	x = x * PI / 180.0f;

	float sin = sinf(x);
	float cos = cosf(x);

	float y = -(sin)+cos;
	float z = cos + sin;
	

	v = Vector3(0.0f, y, z);
	return v;
}

Vector3 BaseObject::RotateY(float y)
{
	Vector3 v;

	y = y + 45.0f;
	y = y * PI / 180.0f;

	float sin = sinf(y);
	float cos = cosf(y);

	float x = cos + sin;
	float z = -(sin)+cos;

	v = Vector3(x, 0.0f, z);
	return v;
}

Vector3 BaseObject::RotateZ(float z)
{
	return Vector3();
}
