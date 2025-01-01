#pragma once

#include "stdafx.h"

struct vec3d // 3D vector
{
	float x, y, z;
};

//Calculate the dot product of two vectors
float DotProduct(vec3d& v1, vec3d& v2);

//Calculate the cross product of two vectors
vec3d CrossProduct(vec3d& v1, vec3d& v2);

//Normalize a vector
vec3d Normalize(vec3d& v);

