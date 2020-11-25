#include "Vector4.h"

const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::one(1.0f, 1.0f, 1.0f, 1.0f);

Vector4::Vector4():x(0.0f),y(0.0f),z(0.0f),w(0.0f)
{
}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

Vector4::~Vector4()
{
}

Vector4::Vector4(const Vector3 & vec3, float w) : x(vec3.x), y(vec3.y), z(vec3.z), w(w)
{
}

Vector4 & Vector4::operator=(const Vector4 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
	return *this;
}
