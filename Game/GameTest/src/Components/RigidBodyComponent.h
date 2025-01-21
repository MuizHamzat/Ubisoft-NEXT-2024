#pragma once

#include "stdafx.h"
#include "src/Math/quat.h"
#include "src/Physics/physics.h"

class RigidBodyComponent
{
public:
	float inverseMass;
	vec3d position;
	quat orientation;
	vec3d velocity;
	vec3d rotation;

	mat3x4 transformMatrix;
	mat3x3 inverseInertiaTensor;
	mat3x3 inverseInertiaTensorWorld;

	void calculateDerivedData()
	{
		_calculateTransformMatrix(transformMatrix, position, orientation);
		//Calculate the inertiaTensor in world space
		_transformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
	}

	static inline void _calculateTransformMatrix(mat3x4& transformMatrix, const vec3d& position, const quat& orientation)
	{
		transformMatrix.data[0][0] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
		transformMatrix.data[0][1] = 2 * orientation.i * orientation.j + 2 * orientation.k * orientation.r;
		transformMatrix.data[0][2] = 2 * orientation.i * orientation.k - 2 * orientation.j * orientation.r;
		transformMatrix.data[0][3] = position.x;
		transformMatrix.data[1][0] = 2 * orientation.i * orientation.j - 2 * orientation.k * orientation.r;
		transformMatrix.data[1][1] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k;
		transformMatrix.data[1][2] = 2 * orientation.j * orientation.k + 2 * orientation.i * orientation.r;
		transformMatrix.data[1][3] = position.y;
		transformMatrix.data[2][0] = 2 * orientation.i * orientation.k + 2 * orientation.j * orientation.r;
		transformMatrix.data[2][1] = 2 * orientation.j * orientation.k - 2 * orientation.i * orientation.r;
		transformMatrix.data[2][2] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j;
		transformMatrix.data[2][3] = position.z;
	}

	void setInertiaTensor(const mat3x3& inertiaTensor)
	{
		inverseInertiaTensor.setInverse(inertiaTensor);
	}

	static inline void _transformInertiaTensor(mat3x3& iitWorld, const quat& q, const mat3x3& iitBody, const mat3x4& rotmat)
	{
		float t4 = rotmat.data[0][0] * iitBody.data[0][0] + rotmat.data[0][1] * iitBody.data[1][0] + rotmat.data[0][2] * iitBody.data[2][0];
		float t9 = rotmat.data[0][0] * iitBody.data[0][1] + rotmat.data[0][1] * iitBody.data[1][1] + rotmat.data[0][2] * iitBody.data[2][1];
		float t14 = rotmat.data[0][0] * iitBody.data[0][2] + rotmat.data[0][1] * iitBody.data[1][2] + rotmat.data[0][2] * iitBody.data[2][2];
		float t28 = rotmat.data[0][0] * iitBody.data[0][0] + rotmat.data[0][1] * iitBody.data[1][0] + rotmat.data[0][2] * iitBody.data[2][0];
		float t33 = rotmat.data[1][0] * iitBody.data[0][0] + rotmat.data[1][1] * iitBody.data[1][0] + rotmat.data[1][2] * iitBody.data[2][0];
		float t38 = rotmat.data[1][0] * iitBody.data[0][1] + rotmat.data[1][1] * iitBody.data[1][1] + rotmat.data[1][2] * iitBody.data[2][1];
		float t52 = rotmat.data[1][0] * iitBody.data[0][0] + rotmat.data[1][1] * iitBody.data[1][0] + rotmat.data[1][2] * iitBody.data[2][0];
		float t57 = rotmat.data[2][0] * iitBody.data[0][0] + rotmat.data[2][1] * iitBody.data[1][0] + rotmat.data[2][2] * iitBody.data[2][0];
		float t62 = rotmat.data[2][0] * iitBody.data[0][1] + rotmat.data[2][1] * iitBody.data[1][1] + rotmat.data[2][2] * iitBody.data[2][1];

		iitWorld.data[0][0] = t4 * rotmat.data[0][0] + t9 * rotmat.data[0][1] + t14 * rotmat.data[0][2];
		iitWorld.data[0][1] = t4 * rotmat.data[1][0] + t9 * rotmat.data[1][1] + t14 * rotmat.data[1][2];
		iitWorld.data[0][2] = t4 * rotmat.data[2][0] + t9 * rotmat.data[2][1] + t14 * rotmat.data[2][2];
		iitWorld.data[1][0] = t28 * rotmat.data[0][0] + t33 * rotmat.data[0][1] + t38 * rotmat.data[0][2];
		iitWorld.data[1][1] = t28 * rotmat.data[1][0] + t33 * rotmat.data[1][1] + t38 * rotmat.data[1][2];
		iitWorld.data[1][2] = t28 * rotmat.data[2][0] + t33 * rotmat.data[2][1] + t38 * rotmat.data[2][2];
		iitWorld.data[2][0] = t52 * rotmat.data[0][0] + t57 * rotmat.data[0][1] + t62 * rotmat.data[0][2];
		iitWorld.data[2][1] = t52 * rotmat.data[1][0] + t57 * rotmat.data[1][1] + t62 * rotmat.data[1][2];
		iitWorld.data[2][2] = t52 * rotmat.data[2][0] + t57 * rotmat.data[2][1] + t62 * rotmat.data[2][2];
	}

};
