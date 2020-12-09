#include "Easing.h"
#include<math.h>

#define PI 3.14

			/*　イーズイン　*/

double Easing::ease_in_sine(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 1 - cos((x * PI) / 2);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_in_quad(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x * x;

	double ret = c * v + s;
	return ret;
}

double Easing::ease_in_cubic(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x * x *x;

	double ret = c * v + s;
	return ret;

}

double Easing::ease_in_quart(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x * x *x *x;

	double ret = c * v + s;
	return ret;
}

double Easing::ease_in_quint(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x * x *x * x*x;

	double ret = c * v + s;
	return ret;
}

double Easing::ease_in_expo(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x == 0 ? 0 : pow(2, 10 * x - 10);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_in_circ(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 1 - sqrt(1 - pow(x, 2));

	double ret = c * v + s;
	return ret;
}

double Easing::ease_in_back(double t, double s, double c, double d)
{
	double c1 = 1.70158;
	double c3 = c1 + 1;

	double x = t / d;
	double v = (c3 *x *x *x) - (c1 *x *x);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_in_elastic(double t, double s, double c, double d)
{

	double c4 = (2 * 3.14) / 3;
	double x = t / d;
	double v = 0;


	if (x == 0)	{

		v = 0;
	}
	else if (x == 1) {

		v = 1;
	}
	else {
		v = pow(2, 10 * x - 10) * sin((x * 10 - 10.75)*c4);

	}
	double ret = c * v + s;

	return ret;
}

double Easing::ease_in_bounce(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 1 - ease_out_bounce(1 - x);

	double ret = c * v + s;

	return ret;
}


			/*　イーズアウト　*/
double Easing::ease_out_sine(double t, double s, double c, double d)
{
	double x = t / d;
	double v = sin((x * PI) / 2);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_quad(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 1 - (1 - x) * (1 - x);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_cubic(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 1 - pow(1 - x, 3);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_quart(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 1 - pow(1 - x, 4);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_quint(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 1 - pow(1 - x, 5);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_expo(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x == 1 ? 1 : 1 - pow(2, -10 * x);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_circ(double t, double s, double c, double d)
{
	double x = t / d;
	double v = sqrt(1 - pow(x - 1, 2));

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_back(double t, double s, double c, double d)
{
	double c1 = 1.70158;
	double c3 = c1 + 1;
	double x = t / d;
	double v = 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_out_elastic(double t, double s, double c, double d)
{
	double c4 = (2 * PI) / 3;
	double x = t / d;
	double v = 0;
	   
	if (x == 0) {

		v = 0;
	}
	else if (x == 1) {

		v = 1;
	}
	else {

		v = pow(2, -10 * x) * sin((x * 10 - 0.75)*c4);
	}
	double ret = c * v + s;

	return ret;
}

double Easing::ease_out_bounce(double t, double s, double c, double d)
{
	double n1 = 7.5625;
	double d1 = 2.75;
	double x = t / d;
	double v = 0;


	if (x < 1 / d1)	{

		v = n1 * x *x;
	}
	else if(x < 2 / d1){

		v = n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {

		v = n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {

		v = n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
	double ret = c * v + s;

	return ret;
}

double Easing::ease_out_bounce(double x)
{
	double n1 = 7.5625;
	double d1 = 2.75;
	double ret = 0;


	if (x < 1 / d1) {

		ret = n1 * x *x;
	}
	else if (x < 2 / d1) {

		ret = n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {

		ret = n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {

		ret = n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
	return ret;
}


				/*　イーズインアウト　*/
double Easing::ease_inout_sine(double t, double s, double c, double d)
{
	double x = t / d;
	double v = -((cos(PI * x) - 1) / 2);

	double ret = c * v + s;
	return ret;
}

double Easing::ease_inout_quad(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x < 0.5 ?
		2 * x * x :
		1 - pow(-2 * x + x, 2) / 2;

	double ret = c * v + s;
	return ret;

}

double Easing::ease_inout_cubic(double t, double s, double c, double d)
{
	double x = t / d;
	double v = (x < 0.5) ?
		4 * x*x*x :
		1 - pow(-2 * x + 2, 3) / 2;

	double ret = c * v + s;
	return ret;
}

double Easing::ease_inout_quart(double t, double s, double c, double d)
{
	double x = t / d;
	double v = (x < 0.5) ?
		8 * x *x *x *x :
		1 - pow(-2 * x + 2, 4) / 2;

	double ret = c * v + s;
	return ret;

}

double Easing::ease_inout_quint(double t, double s, double c, double d)
{
	double x = t / d;
	double v = (x < 0.5) ?
		16 * x *x *x *x *x :
		1 - pow(-2 * x + 2, 5) / 2;

	double ret = c * v + s;
	return ret;
}

double Easing::ease_inout_expo(double t, double s, double c, double d)
{
	double x = t / d;
	double v = 0;
	
	if (x == 0) {

		v = 0;
	}
	else if(x == 1) {

		v = 1;
	}
	else if (x < 0.5) {

		v = pow(2,20 * x - 10) / 2;
	}
	else {
		v = (2 - pow(2, -20 * x + 10)) / 2;
	}
	double ret = c * v + s;

	return ret;
}

double Easing::ease_inout_circ(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x < 0.5 ?
		(1 - sqrt(1 - pow(2 * x, 2))) / 2 :
		(sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;

	double ret = c * v + s;
	return ret;
}

double Easing::ease_inout_back(double t, double s, double c, double d)
{
	double c1 = 1.70158;
	double c2 = c1 * 1.525;

	double x = t / d;
	double v = x < 0.5 ?
		(pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2 :
		(pow(2 * x - 2, 2) * ((c2 + 1) *(x * 2 - 2) + c2) + c2) / 2;

	double ret = c * v + s;
	return ret;
}

double Easing::ease_inout_elastic(double t, double s, double c, double d)
{
	double c5 = (2 * 3.14) / 4.5;
	double x = t / d;
	double v = 0;
	
	if (x == 0) {
		v = 0;

	}
	else if (x == 1) {
		v = 1;
	}
	else if (x < 0.5) {
		v = -(pow(2, 20 * x - 10) * sin((x * 10 - 11.125)*c5)) / 2;

	}
	else {
		v = (pow(2, -20 * x + 10) * sin((x * 10 - 11.125)*c5)) / 2 + 1;
	}
	double ret = c * v + s;

	return ret;
}

double Easing::ease_inout_bounce(double t, double s, double c, double d)
{
	double x = t / d;
	double v = x < 0.5 ?
		(1 - ease_out_bounce(1 - 2 * x)) / 2 :
		(1 + ease_out_bounce(2 * x - 1)) / 2;

	double ret = c * v + s;
	return ret;

}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

/*　イーズイン　*/


Vector3 Easing::ease_in_sine(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = 1 - cos((x * PI) / 2);

	Vector3 ret = c * v + s;
	return ret;

}

Vector3 Easing::ease_in_quad(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = x * x;

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_in_cubic(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = x * x *x;

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_in_quart(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = x * x *x *x;

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_in_quint(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = x == 0 ? 0 : pow(2, 10 * x - 10);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_in_expo(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = x == 0 ? 0 : pow(2, 10 * x - 10);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_in_circ(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = 1 - sqrt(1 - pow(x, 2));

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_in_back(double t, Vector3 s, Vector3 c, double d)
{
	double c1 = 1.70158;
	double c3 = c1 + 1;

	double x = t / d;
	double v = (c3 *x *x *x) - (c1 *x *x);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_in_elastic(double t, Vector3 s, Vector3 c, double d)
{
	double c4 = (2 * 3.14) / 3;
	double x = t / d;
	double v = 0;


	if (x == 0) {

		v = 0;
	}
	else if (x == 1) {

		v = 1;
	}
	else {
		v = pow(2, 10 * x - 10) * sin((x * 10 - 10.75)*c4);

	}
	Vector3 ret = c * v + s;

	return ret;
}

Vector3 Easing::ease_in_bounce(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = 1 - ease_out_bounce(1 - x);

	Vector3 ret = c * v + s;

	return ret;
}


/*　イーズアウト　*/


Vector3 Easing::ease_out_sine(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = sin((x * PI) / 2);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_quad(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = 1 - (1 - x) * (1 - x);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_cubic(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = 1 - pow(1 - x, 3);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_quart(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = 1 - pow(1 - x, 4);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_quint(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = 1 - pow(1 - x, 5);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_expo(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = x == 1 ? 1 : 1 - pow(2, -10 * x);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_circ(double t, Vector3 s, Vector3 c, double d)
{
	double x = t / d;
	double v = sqrt(1 - pow(x - 1, 2));

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_back(double t, Vector3 s, Vector3 c, double d)
{
	double c1 = 1.70158;
	double c3 = c1 + 1;
	double x = t / d;
	double v = 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);

	Vector3 ret = c * v + s;
	return ret;
}

Vector3 Easing::ease_out_elastic(double t, Vector3 s, Vector3 c, double d)
{
	double c4 = (2 * PI) / 3;
	double x = t / d;
	double v = 0;

	if (x == 0) {

		v = 0;
	}
	else if (x == 1) {

		v = 1;
	}
	else {

		v = pow(2, -10 * x) * sin((x * 10 - 0.75)*c4);
	}
	Vector3 ret = c * v + s;

	return ret;
}

Vector3 Easing::ease_out_bounce(double t, Vector3 s, Vector3 c, double d)
{
	double n1 = 7.5625;
	double d1 = 2.75;
	double x = t / d;
	double v = 0;


	if (x < 1 / d1) {

		v = n1 * x *x;
	}
	else if (x < 2 / d1) {

		v = n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {

		v = n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {

		v = n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
	Vector3 ret = c * v + s;

	return ret;
}
