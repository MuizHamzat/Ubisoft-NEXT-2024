#pragma once

#include "stdafx.h"
#include "vec3d.h"

struct mat4x4 // 4x4 matrix
{
	float m[4][4] = { 0 };
};

//MATRIX OPERATIONS

// Multiply a vector and matrix
vec3d MatrixMultiplyVector(mat4x4& m, vec3d& i);

// Multiply two matrices
mat4x4 MatrixMultiplyMatrix(mat4x4& m1, mat4x4& m2);

// Create a matrix that points at a target
mat4x4 MatrixPointAt(vec3d& pos, vec3d& target, vec3d& up);

// Create a quick inverse matrix only for Rotation/Translation matrices
mat4x4 MatrixQuickInverse(mat4x4& m);

//CREATING MATRICES

//Transforms

//Make a scaling matrix
mat4x4 MatrixMakeScaling(float x, float y, float z);

// Make a rotation matrix around the x axis
mat4x4 MatrixMakeRotationX(float fAngleRad);

// Make a rotation matrix around the y axis
mat4x4 MatrixMakeRotationY(float fAngleRad);

// Make a rotation matrix around the z axis
mat4x4 MatrixMakeRotationZ(float fAngleRad);

mat4x4 MatrixMakeRotation(vec3d& rotation);

//Make a translation matrix
mat4x4 MatrixMakeTranslation(float x, float y, float z);

//Misc

//Make a projection matrix (to project 3D coordinates to 2D coordinates)
mat4x4 MatrixMakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

//Make an Identity matrix
mat4x4 MatrixMakeIdentity();
