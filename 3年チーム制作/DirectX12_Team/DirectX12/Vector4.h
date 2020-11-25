#pragma once
#include "Vector3.h"
class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
	Vector4();
	Vector4(float x, float y,float z,float w);
	~Vector4();
	Vector4(const Vector3&vec3, float w);

	Vector4& operator =(const Vector4& vec);

	static const Vector4 zero;
	static const Vector4 one;
private:

};
