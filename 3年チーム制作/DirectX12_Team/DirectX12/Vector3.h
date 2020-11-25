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
	float dot(const Vector3& v)const;//内積
	static float dot(const Vector3& v,const Vector3& v2);//内積
	Vector3 cross(const Vector3&v)const;//外積
	static Vector3 cross(const Vector3&v,const Vector3& v2);//外積
	//単項演算子オーバーロード
	Vector3 operator +()const;
	Vector3 operator -()const;
	//代入演算子オーバーロード
	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *=(float s);
	Vector3& operator /=(float s);

	static const Vector3 zero;
	static const Vector3 one;
private:

};
//二項演算子オーバーロード
const Vector3 operator + (const Vector3& v1, const Vector3& v2);
const Vector3 operator - (const Vector3& v1, const Vector3& v2);
const Vector3 operator * (const Vector3& v, float s);
const Vector3 operator * (float s, const Vector3& v);
const Vector3 operator / (const Vector3& v, float s);
