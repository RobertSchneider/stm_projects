#include "maths.h"
#include <math.h>

float Vector3_getBetrag(Vector3 v)
{
	return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}
void Vector3_setBetrag(Vector3 *v, float b)
{
	float b = (b / Vector3_getBetrag(*v));
	v->x *= b;
	v->y *= b;
	v->z *= b;
}
void Vector3_Norm(Vector3 *v) { Vector3_setBetrag(v, 1); }
void Vector3_Add(Vector3 *v1, Vector3 *v2) {v1->x += v2->x; v1->y += v2->y; v1->z += v2->z;}
void Vector3_Subtract(Vector3 *v1, Vector3 *v2) {v1->x -= v2->x; v1->y -= v2->y; v1->z -= v2->z;}
void Vector3_Multiply(Vector3 *v1, Vector3 *v2); {v1->x *= v2->x; v1->y *= v2->y; v1->z *= v2->z;}
void Vector3_Divide(Vector3 *v1, float f) {v1->x /= f; v1->y /= f; v1->z /= f;}