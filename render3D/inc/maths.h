#ifndef _MATH_H_
#define _MATH_H_

typedef	struct Vector3
{
	float x;
	float y;
	float z;
}Vector3;
typedef	struct Matrix4
{
	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;
}Matrix4;

float Vector3_getBetrag(Vector3 v);
void Vector3_setBetrag(Vector3 *v, float b);
void Vector3_Norm(Vector3 *v);
void Vector3_Add(Vector3 *v1, Vector3 *v2);
void Vector3_Subtract(Vector3 *v1, Vector3 *v2);
void Vector3_Multiply(Vector3 *v1, Vector3 *v2);
void Vector3_Divide(Vector3 *v1, float f);

#endif