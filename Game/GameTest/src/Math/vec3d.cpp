#include "stdafx.h"
#include "vec3d.h"

float DotProduct(vec3d& v1, vec3d& v2)
{
	vec3d result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result.x + result.y + result.z;
}

vec3d CrossProduct(vec3d& v1, vec3d& v2)
{
	vec3d result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

vec3d Normalize(vec3d& v)
{
	float l = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	vec3d result;
	result.x = v.x / l;
	result.y = v.y / l;
	result.z = v.z / l;
	return result;
}

vec3d VectorAdd(vec3d& v1, vec3d& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

vec3d VectorSubtract(vec3d& v1, vec3d& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

vec3d VectorMultiply(vec3d& v, float scalar)
{
	return { v.x * scalar, v.y * scalar, v.z * scalar };
}

vec3d VectorDivide(vec3d& v, float scalar)
{
	return { v.x / scalar, v.y / scalar, v.z / scalar };
}