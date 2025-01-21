#pragma once

#include "stdafx.h"
#include "src/Math/quat.h"


struct mat3x3 // 3x3 matrix
{
	float data[3][3] = { 0 };

	mat3x3 operator*(const mat3x3& rhs) const
	{
		mat3x3 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result.data[i][j] = data[i][0] * rhs.data[0][j] + data[i][1] * rhs.data[1][j] + data[i][2] * rhs.data[2][j];
			}
		}
		return result;
	}

	// Sets the matrix to be the inverse of the given matrix
	void setInverse(const mat3x3& m)
	{
		float t4 = m.data[0][0] * m.data[1][1];
		float t6 = m.data[0][0] * m.data[1][2];
		float t8 = m.data[0][1] * m.data[1][0];
		float t10 = m.data[0][2] * m.data[1][0];
		float t12 = m.data[0][1] * m.data[1][2];
		float t14 = m.data[0][2] * m.data[1][1];

		// Calculate the determinant
		float t16 = (t4 * m.data[2][2] - t6 * m.data[2][1] - t8 * m.data[2][2] +
			t10 * m.data[2][1] + t12 * m.data[2][0] - t14 * m.data[2][0]);

		// Make sure the determinant is non-zero.
		if (t16 == 0.0f) return;
		float t17 = 1 / t16;
		data[0][0] = (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]) * t17;
		data[0][1] = -(m.data[0][1] * m.data[2][2] - m.data[0][2] * m.data[2][1]) * t17;
		data[0][2] = (m.data[0][1] * m.data[1][2] - m.data[0][2] * m.data[1][1]) * t17;
		data[1][0] = -(m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0]) * t17;
		data[1][1] = (m.data[0][0] * m.data[2][2] - t14) * t17;
		data[1][2] = -(t6 - t8) * t17;
		data[2][0] = (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]) * t17;
		data[2][1] = -(m.data[0][0] * m.data[2][1] - t12) * t17;
		data[2][2] = (t4 - t8) * t17;
	}

	//Returns a new matrix containing the inverse of this matrix
	mat3x3 inverse() const
	{
		mat3x3 result;
		result.setInverse(*this);
		return result;
	}

	//Inverts the matrix
	void invert()
	{
		setInverse(*this);
	}

	//Sets the matrix to be the transpose of the given matrix
	void setTranspose(const mat3x3& m)
	{
		data[0][0] = m.data[0][0];
		data[0][1] = m.data[1][0];
		data[0][2] = m.data[2][0];
		data[1][0] = m.data[0][1];
		data[1][1] = m.data[1][1];
		data[1][2] = m.data[2][1];
		data[2][0] = m.data[0][2];
		data[2][1] = m.data[1][2];
		data[2][2] = m.data[2][2];
	}

	//Returns a new matrix containing the transpose of this matrix
	mat3x3 transpose() const
	{
		mat3x3 result;
		result.setTranspose(*this);
		return result;
	}

	//Sets the matrix to be the rotation matrix for the given quaternion
	void setOrientation(const quat& q)
	{
		data[0][0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
		data[0][1] = 2 * q.i * q.j + 2 * q.k * q.r;
		data[0][2] = 2 * q.i * q.k - 2 * q.j * q.r;
		data[1][0] = 2 * q.i * q.j - 2 * q.k * q.r;
		data[1][1] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
		data[1][2] = 2 * q.j * q.k + 2 * q.i * q.r;
		data[2][0] = 2 * q.i * q.k + 2 * q.j * q.r;
		data[2][1] = 2 * q.j * q.k - 2 * q.i * q.r;
		data[2][2] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
	}
};

struct mat3x4 // 3x4 matrix
{
	float data[3][4] = { 0 };

	vec3d operator*(const vec3d& rhs) const
	{
		vec3d result;
		result.x = data[0][0] * rhs.x + data[0][1] * rhs.y + data[0][2] * rhs.z + data[0][3];
		result.y = data[1][0] * rhs.x + data[1][1] * rhs.y + data[1][2] * rhs.z + data[1][3];
		result.z = data[2][0] * rhs.x + data[2][1] * rhs.y + data[2][2] * rhs.z + data[2][3];
		return result;
	}

	mat3x4 operator*(const mat3x4& rhs) const
	{
		mat3x4 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.data[i][j] = data[i][0] * rhs.data[0][j] + data[i][1] * rhs.data[1][j] + data[i][2] * rhs.data[2][j];
			}
		}
		return result;
	}

	//Returns the determinant of the matrix
	float getDeterminant() const
	{
		return data[2][0] * data[1][1] * data[0][2] +
			data[1][0] * data[2][1] * data[0][2] +
			data[2][2] * data[0][1] * data[1][2] -
			data[0][0] * data[2][1] * data[1][2] -
			data[1][0] * data[0][1] * data[2][2] +
			data[0][0] * data[1][1] * data[2][2];
	}

	//Sets the matrix to be the inverse of the given matrix
	void setInverse(const mat3x4& m)
	{
		//Make sure the determinant is non-zero
		float det = m.getDeterminant();
		if (det == 0) return;
		det = 1.0f / det;
		data[0][0] = (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]) * det;
		data[0][1] = (m.data[0][2] * m.data[2][1] - m.data[0][1] * m.data[2][2]) * det;
		data[0][2] = (m.data[0][1] * m.data[1][2] - m.data[0][2] * m.data[1][1]) * det;
		data[1][0] = (m.data[1][2] * m.data[2][0] - m.data[1][0] * m.data[2][2]) * det;
		data[1][1] = (m.data[0][0] * m.data[2][2] - m.data[0][2] * m.data[2][0]) * det;
		data[1][2] = (m.data[0][2] * m.data[1][0] - m.data[0][0] * m.data[1][2]) * det;
		data[2][0] = (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]) * det;
		data[2][1] = (m.data[0][1] * m.data[2][0] - m.data[0][0] * m.data[2][1]) * det;
		data[2][2] = (m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0]) * det;
	}

	//Returns a new matrix containing the inverse of this matrix
	mat3x4 inverse() const
	{
		mat3x4 result;
		result.setInverse(*this);
		return result;
	}

	//Inverts the matrix
	void invert()
	{
		setInverse(*this);
	}

	//Sets this matrix to be the rotation matrix for the given quaternion
	void setOrientationAndPos(const quat& q, const vec3d& pos)
	{
		data[0][0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
		data[0][1] = 2 * q.i * q.j + 2 * q.k * q.r;
		data[0][2] = 2 * q.i * q.k - 2 * q.j * q.r;
		data[1][0] = 2 * q.i * q.j - 2 * q.k * q.r;
		data[1][1] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
		data[1][2] = 2 * q.j * q.k + 2 * q.i * q.r;
		data[2][0] = 2 * q.i * q.k + 2 * q.j * q.r;
		data[2][1] = 2 * q.j * q.k - 2 * q.i * q.r;
		data[2][2] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
		data[0][3] = pos.x;
		data[1][3] = pos.y;
		data[2][3] = pos.z;
	}

	//Transforms the given vector by the transformational inverse of this matrix
	vec3d transformInverse(const vec3d& vector) const
	{
		vec3d tmp = vector;
		tmp.x -= data[0][3];
		tmp.y -= data[1][3];
		tmp.z -= data[2][3];

		vec3d result;
		result.x = tmp.x * data[0][0] + tmp.y * data[1][0] + tmp.z * data[2][0];
		result.y = tmp.x * data[0][1] + tmp.y * data[1][1] + tmp.z * data[2][1];
		result.z = tmp.x * data[0][2] + tmp.y * data[1][2] + tmp.z * data[2][2];

		return result;
	}

	//Transforms the given direction vector by this matrix
	vec3d transformDirection(const vec3d& vector) const
	{
		vec3d result;
		result.x = vector.x * data[0][0] + vector.y * data[1][0] + vector.z * data[2][0];
		result.y = vector.x * data[0][1] + vector.y * data[1][1] + vector.z * data[2][1];
		result.z = vector.x * data[0][2] + vector.y * data[1][2] + vector.z * data[2][2];
		return result;
	}

	//Transforms the given direction vector by the transformational inverse of this matrix
	vec3d transformInverseDirection(const vec3d& vector) const
	{
		vec3d result;
		result.x = vector.x * data[0][0] + vector.y * data[0][1] + vector.z * data[0][2];
		result.y = vector.x * data[1][0] + vector.y * data[1][1] + vector.z * data[1][2];
		result.z = vector.x * data[2][0] + vector.y * data[2][1] + vector.z * data[2][2];
		return result;
	}
};
