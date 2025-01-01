#pragma once

#include "stdafx.h"
#include "vec3d.h"

struct mat4x4 // 4x4 matrix
{
	float m[4][4] = { 0 };
};

// Multiply a vector and matrix
void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m);
