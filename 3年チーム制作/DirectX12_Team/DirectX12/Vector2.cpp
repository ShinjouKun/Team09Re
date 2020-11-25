#include "Vector2.h"
#include<cmath>

Vector2::Vector2():x(0),y(0)
{
}

Vector2::Vector2(float x, float y):x(x),y(y)
{
}

Vector2::~Vector2()
{
}

float Vector2::Length() const
{
	return (float)std::sqrtf(x*x+y*y);
}

Vector2 & Vector2::normalize()
{
	float Len;
	if ((Len = Length()) != 0)
		*this /= Len;
	return *this;
}

float Vector2::dot(const Vector2 & v) const
{
	return x*v.y - y*v.x;
}

float Vector2::cross(const Vector2 & v) const
{
	return x*v.y - y * v.x;
}

Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x,-y);
}

Vector2 & Vector2::operator+=(const Vector2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 & Vector2::operator-=(const Vector2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 & Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2 & Vector2::operator/=(float s)
{
	if (s == 0)
	{
		x = x;
		y = y;
	}
	else
	{
		x /= s;
		y /= s;
	}
	return *this;
}

const Vector2 operator+(const Vector2 & v1, const Vector2 & v2)
{
	return Vector2(v1.x + v2.x,v1.y + v2.y);
}

const Vector2 operator-(const Vector2 & v1, const Vector2 & v2)
{
	return Vector2(v1.x -v2.x,v1.y - v2.y);
}

const Vector2 operator*(const Vector2 & v, float s)
{
	return Vector2(v.x *s,v.y*s);
}

const Vector2 operator*(float s, const Vector2 & v)
{
	return Vector2(v.x*s,v.y*s);
}

const Vector2 operator/(const Vector2 & v, float s)
{
	return Vector2(v.x/s,v.y/s);
}
