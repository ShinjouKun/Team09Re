#include "Matrix4.h"
#include<memory>
#include<cmath>


const float PI = 3.1415926535f;
const float degree = PI / 180.0f;
float Cos(float deg)
{
	return cosf(deg*degree);
}
float Sin(float deg)
{
	return sinf(deg*degree);
}
float m4Ident[4][4] =
{
	{1.0f,0.0f,0.0f,0.0f },
	{0.0f,1.0f,0.0f,0.0f },
	{0.0f,0.0f,1.0f,0.0f },
	{0.0f,0.0f,0.0f,1.0f }
};
const Matrix4 Matrix4::Identity(m4Ident);
Matrix4::Matrix4()
{
	*this = Matrix4::Identity;
}

Matrix4::~Matrix4()
{
}

Matrix4::Matrix4(float mat[4][4])
{
	memcpy(m, mat, 16 * sizeof(float));
}

Matrix4 & Matrix4::operator*=(const Matrix4 & a)
{
	*this = *this*a;
	return *this;
}

Matrix4 Matrix4::createTranslation(const Vector3 & trans)
{
	float temp[4][4]=
	{
	{1.0f,0.0f,0.0f,0.0f },
	{0.0f,1.0f,0.0f,0.0f },
	{0.0f,0.0f,1.0f,0.0f },
	{trans.x,trans.y,trans.z,1.0f}
	};
	return Matrix4(temp);
}

Matrix4 Matrix4::createScale(float x, float y, float z)
{
  float temp[4][4] =
  {
	 {x, 0.0f, 0.0f, 0.0f },
	{ 0.0f,y,0.0f,0.0f },
	{ 0.0f,0.0f,z,0.0f },
	{ 0.0f,0.0f,0.0f,1.0f }
  };

  return Matrix4(temp);
}

Matrix4 Matrix4::createScale(const Vector3 & scale)
{
	return createScale(scale.x,scale.y,scale.y);
}

Matrix4 Matrix4::Rotate(float x,float y,float z)
{
	float temp[4][4] = 
	{
		{Cos(x),Sin(y),z,0.0f},
		{Cos(x),Sin(y),z,0.0f},
		{Cos(x),Sin(y),z,0.0f},
		{Cos(x),Sin(y),z,0.0f},
	};
	return Matrix4(temp);
}

Matrix4 Matrix4::RotateX(float angleX)
{
	float temp[4][4] =
	{
		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,Cos(angleX),-Sin(angleX),0.0f},
		{0.0f,Sin(angleX),Cos(angleX),0.0f},
		{0.0f,0.0f,0.0f,1.0f }
	};
	return Matrix4(temp);
}

Matrix4 Matrix4::RotateY(float angleY)
{
	float temp[4][4]=
	{
		{Cos(angleY),0.0f,Sin(angleY),0.0f},
	    {0.0f,1.0f,0.0f,0.0f},
		{-Sin(angleY),0.0f,Cos(angleY),0.0f},
		{0.0f,0.0f,0.0f,1.0f }
	};
	return Matrix4(temp);
}

Matrix4 Matrix4::RotateZ(float angleZ)
{
	float temp[4][4]=
	{
		{Cos(angleZ),Sin(angleZ),0.0f,0.0f},
		{-Sin(angleZ),Cos(angleZ),0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f },
	  {0.0f,0.0f,0.0f,1.0f }
	};

	return Matrix4(temp);
}

Matrix4 Matrix4::createLookAt(const Vector3 & eye, const Vector3 & target, const Vector3 & up)
{
	Vector3 zAxis = Vector3::normalize(target - eye);
	Vector3 xAxis = Vector3::normalize(Vector3::cross(up,zAxis));
	Vector3 yAxis = Vector3::normalize(Vector3::cross(zAxis, xAxis));
	Vector3 trans;
	trans.x = -Vector3::dot(xAxis,eye);
	trans.y = -Vector3::dot(yAxis, eye);
	trans.z = -Vector3::dot(zAxis, eye);

	float temp[4][4]=
	{
		{xAxis.x,yAxis.x,zAxis.x,0.0f},
		{xAxis.y,yAxis.y,zAxis.y,0.0f},
	    {xAxis.z,yAxis.z,zAxis.z,0.0f},
	    {trans.x,trans.y,trans.z,1.0f}
	};

	return Matrix4(temp);
}

Matrix4 operator*(const Matrix4 & a, const Matrix4 & b)
{
	Matrix4 temp;

	temp.m[0][0] =
		a.m[0][0] * b.m[0][0] +
		a.m[0][1] * b.m[1][0] +
		a.m[0][2] * b.m[2][0] +
		a.m[0][3] * b.m[3][0];

	temp.m[0][1] =
		a.m[0][0] * b.m[0][1] +
		a.m[0][1] * b.m[1][1] +
		a.m[0][2] * b.m[2][1] +
		a.m[0][3] * b.m[3][1];

	temp.m[0][2] =
		a.m[0][0] * b.m[0][2] +
		a.m[0][1] * b.m[1][2] +
		a.m[0][2] * b.m[2][2] +
		a.m[0][3] * b.m[3][2];

	temp.m[0][3] =
		a.m[0][0] * b.m[0][3] +
		a.m[0][1] * b.m[1][3] +
		a.m[0][2] * b.m[2][3] +
		a.m[0][3] * b.m[3][3];

	temp.m[1][0] =
		a.m[1][0] * b.m[0][0] +
		a.m[1][1] * b.m[1][0] +
		a.m[1][2] * b.m[2][0] +
		a.m[1][3] * b.m[3][0];

	temp.m[1][1] =
		a.m[1][0] * b.m[0][1] +
		a.m[1][1] * b.m[1][1] +
		a.m[1][2] * b.m[2][1] +
		a.m[1][3] * b.m[3][1];

	temp.m[1][2] =
		a.m[1][0] * b.m[0][2] +
		a.m[1][1] * b.m[1][2] +
		a.m[1][2] * b.m[2][2] +
		a.m[1][3] * b.m[3][2];

	temp.m[1][3] =
		a.m[1][0] * b.m[0][3] +
		a.m[1][1] * b.m[1][3] +
		a.m[1][2] * b.m[2][3] +
		a.m[1][3] * b.m[3][3];

	temp.m[2][0] =
		a.m[2][0] * b.m[0][0] +
		a.m[2][1] * b.m[1][0] +
		a.m[2][2] * b.m[2][0] +
		a.m[2][3] * b.m[3][0];

	temp.m[2][1] =
		a.m[2][0] * b.m[0][1] +
		a.m[2][1] * b.m[1][1] +
		a.m[2][2] * b.m[2][1] +
		a.m[2][3] * b.m[3][1];

	temp.m[2][2] =
		a.m[2][0] * b.m[0][2] +
		a.m[2][1] * b.m[1][2] +
		a.m[2][2] * b.m[2][2] +
		a.m[2][3] * b.m[3][2];

	temp.m[2][3] =
		a.m[2][0] * b.m[0][3] +
		a.m[2][1] * b.m[1][3] +
		a.m[2][2] * b.m[2][3] +
		a.m[2][3] * b.m[3][3];

	temp.m[3][0] =
		a.m[3][0] * b.m[0][0] +
		a.m[3][1] * b.m[1][0] +
		a.m[3][2] * b.m[2][0] +
		a.m[3][3] * b.m[3][0];

	temp.m[3][1] =
		a.m[3][0] * b.m[0][1] +
		a.m[3][1] * b.m[1][1] +
		a.m[3][2] * b.m[2][1] +
		a.m[3][3] * b.m[3][1];

	temp.m[3][2] =
		a.m[3][0] * b.m[0][2] +
		a.m[3][1] * b.m[1][2] +
		a.m[3][2] * b.m[2][2] +
		a.m[3][3] * b.m[3][2];

	temp.m[3][3] =
		a.m[3][0] * b.m[0][3] +
		a.m[3][1] * b.m[1][3] +
		a.m[3][2] * b.m[2][3] +
		a.m[3][3] * b.m[3][3];

	return temp;
}						   
