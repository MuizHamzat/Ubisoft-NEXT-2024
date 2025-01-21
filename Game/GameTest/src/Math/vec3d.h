#pragma once

#include "stdafx.h"

struct vec3d // 3D vector
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1; // w component, used for perspective projection


	//Operator overloads

	// Addition
	vec3d operator+(const vec3d& other) const
	{
		return { x + other.x, y + other.y, z + other.z, w };
	}

	// Subtraction
	vec3d operator-(const vec3d& other) const
	{
		return { x - other.x, y - other.y, z - other.z, w };
	}

	// Multiplication by scalar
	vec3d operator*(float scalar) const
	{
		return { x * scalar, y * scalar, z * scalar, w };
	}

	// Division by scalar
	vec3d operator/(float scalar) const
	{
		return { x / scalar, y / scalar, z / scalar, w };
	}

	// Compound assignment operators
	vec3d& operator+=(const vec3d& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vec3d& operator-=(const vec3d& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	vec3d& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	vec3d& operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
};

// Non-member functions for scalar operations (these are here so that we can write both scalar * vector and vector * scalar)
//vec3d operator*(float scalar, const vec3d& vec)
//{
//	return { vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w };
//}

vec3d operator/(float scalar, const vec3d& vec)
{
	return { scalar / vec.x, scalar / vec.y, scalar / vec.z, vec.w };
}

//Calculate the magnitude of a vector
float Magnitude(vec3d& v);

vec3d ComponentProduct(vec3d& v1, vec3d& v2);

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

