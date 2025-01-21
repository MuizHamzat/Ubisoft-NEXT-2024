#include "stdafx.h"
#include "quat.h"

void quat::normalize()
{
	float d = r * r + i * i + j * j + k * k;
	//Check for zero length quaternion, and use the no-rotation quaternion in that case
	if (d == 0)
	{
		r = 1;
		return;
	}
	d = 1.0f / sqrtf(d);
	r *= d;
	i *= d;
	j *= d;
	k *= d;
}

void quat::operator*= (const quat& multiplier)
{
	quat q = *this;
	r = q.r * multiplier.r - q.i * multiplier.i -
		q.j * multiplier.j - q.k * multiplier.k;

	i = q.r * multiplier.i + q.i * multiplier.r +
		q.j * multiplier.k - q.k * multiplier.j;

	j = q.r * multiplier.j + q.j * multiplier.r +
		q.k * multiplier.i - q.i * multiplier.k;

	k = q.r * multiplier.k + q.k * multiplier.r +
		q.i * multiplier.j - q.j * multiplier.i;
}

//void quat::rotateByVector(const vec3d vector)
//{
//	Quaternion q((float) 0.0f, vector.x, vector.y, vector.z);
//	(*this) *= q;
//}

void quat::addScaledVector(const vec3d& vector, float scale)
{
	quat q;
	q.r = 0;
	q.i = vector.x * scale;
	q.j = vector.y * scale;
	q.k = vector.z * scale;
	q *= *this;
	r += q.r * 0.5f;
	i += q.i * 0.5f;
	j += q.j * 0.5f;
	k += q.k * 0.5f;
}
