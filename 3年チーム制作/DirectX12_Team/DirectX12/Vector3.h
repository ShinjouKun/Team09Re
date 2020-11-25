#pragma once
class Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3();
	Vector3(float x,float y,float z);
	~Vector3();

	float Length()const;
	void normalize();
	static Vector3 normalize(const Vector3& vec);
	float dot(const Vector3& v)const;//����
	static float dot(const Vector3& v,const Vector3& v2);//����
	Vector3 cross(const Vector3&v)const;//�O��
	static Vector3 cross(const Vector3&v,const Vector3& v2);//�O��
	//�P�����Z�q�I�[�o�[���[�h
	Vector3 operator +()const;
	Vector3 operator -()const;
	//������Z�q�I�[�o�[���[�h
	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *=(float s);
	Vector3& operator /=(float s);

	static const Vector3 zero;
	static const Vector3 one;
private:

};
//�񍀉��Z�q�I�[�o�[���[�h
const Vector3 operator + (const Vector3& v1, const Vector3& v2);
const Vector3 operator - (const Vector3& v1, const Vector3& v2);
const Vector3 operator * (const Vector3& v, float s);
const Vector3 operator * (float s, const Vector3& v);
const Vector3 operator / (const Vector3& v, float s);
