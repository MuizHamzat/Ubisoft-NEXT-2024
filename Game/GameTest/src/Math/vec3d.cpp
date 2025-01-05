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

vec3d VectorIntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
{
	plane_n = Normalize(plane_n); // Make sure the normal is normalized
	float plane_d = -DotProduct(plane_n, plane_p); // Get the distance to the plane from the origin
	float ad = DotProduct(lineStart, plane_n); // Get the distance of the line start to the plane
	float bd = DotProduct(lineEnd, plane_n); // Get the distance of the line end to the plane
	float t = (-plane_d - ad) / (bd - ad); // Get the distance from the line start to the intersection point
	vec3d lineStartToEnd = VectorSubtract(lineEnd, lineStart); // Get the vector from the line start to the line end
	vec3d lineToIntersect = VectorMultiply(lineStartToEnd, t); // Get the vector from the line start to the intersection point
	return VectorAdd(lineStart, lineToIntersect); // return the intersection point between the line and plane
}