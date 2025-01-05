#pragma once

#include "stdafx.h"

struct vec3d // 3D vector
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1; // w component, used for perspective projection
};

//Calculate the dot product of two vectors
float DotProduct(vec3d& v1, vec3d& v2);

//Calculate the cross product of two vectors
vec3d CrossProduct(vec3d& v1, vec3d& v2);

//Normalize a vector
vec3d Normalize(vec3d& v);

//Add two vectors
vec3d VectorAdd(vec3d& v1, vec3d& v2);

//Subtract two vectors
vec3d VectorSubtract(vec3d& v1, vec3d& v2);

//Multiply a vector by a scalar
vec3d VectorMultiply(vec3d& v, float scalar);

//Divide a vector by a scalar
vec3d VectorDivide(vec3d& v, float scalar);

//Intersect a line and a plane
vec3d VectorIntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd);

