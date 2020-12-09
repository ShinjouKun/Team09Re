#pragma once
#include <DirectXMath.h>
#include"Vector3.h"

//----------------------
//à¯êî:(double t, double s, double c, double d)
//	t :
//	s :
//	c :
//	d :
//-----------------------
class Easing
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:	
	double static ease_in_sine   (double t, double s, double c, double d);
	double static ease_in_quad   (double t, double s, double c, double d);
	double static ease_in_cubic  (double t, double s, double c, double d);
	double static ease_in_quart  (double t, double s, double c, double d);
	double static ease_in_quint  (double t, double s, double c, double d);
	double static ease_in_expo   (double t, double s, double c, double d);
	double static ease_in_circ   (double t, double s, double c, double d);
	double static ease_in_back   (double t, double s, double c, double d);
	double static ease_in_elastic(double t, double s, double c, double d);
	double static ease_in_bounce (double t, double s, double c, double d);


	double static ease_out_sine   (double t, double s, double c, double d);
	double static ease_out_quad   (double t, double s, double c, double d);
	double static ease_out_cubic  (double t, double s, double c, double d);
	double static ease_out_quart  (double t, double s, double c, double d);
	double static ease_out_quint  (double t, double s, double c, double d);
	double static ease_out_expo   (double t, double s, double c, double d);
	double static ease_out_circ   (double t, double s, double c, double d);
	double static ease_out_back   (double t, double s, double c, double d);
	double static ease_out_elastic(double t, double s, double c, double d);
	double static ease_out_bounce (double t, double s, double c, double d);



	double static ease_inout_sine   (double t, double s, double c, double d);
	double static ease_inout_quad   (double t, double s, double c, double d);
	double static ease_inout_cubic  (double t, double s, double c, double d);
	double static ease_inout_quart  (double t, double s, double c, double d);
	double static ease_inout_quint  (double t, double s, double c, double d);
	double static ease_inout_expo   (double t, double s, double c, double d);
	double static ease_inout_circ   (double t, double s, double c, double d);
	double static ease_inout_back   (double t, double s, double c, double d);
	double static ease_inout_elastic(double t, double s, double c, double d);
	double static ease_inout_bounce (double t, double s, double c, double d);

	/*Å@Vector3Å@*/
	
	Vector3 static ease_in_sine	  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_quad	  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_cubic  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_quart  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_quint  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_expo   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_circ   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_back   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_elastic(double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_in_bounce (double t, Vector3 s, Vector3 c, double d);

	Vector3 static ease_out_sine   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_quad   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_cubic  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_quart  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_quint  (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_expo   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_circ   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_back   (double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_elastic(double t, Vector3 s, Vector3 c, double d);
	Vector3 static ease_out_bounce (double t, Vector3 s, Vector3 c, double d);

private:
	double static ease_out_bounce(double x);



};
