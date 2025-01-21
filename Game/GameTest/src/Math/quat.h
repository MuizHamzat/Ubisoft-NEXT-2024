#pragma once

#include "stdafx.h"

class quat
{
public:
	union
	{
		struct
		{
			//Real component of the quaternion
			float r;
			//Complex components
			float i, j, k;
		};

		//Quaternion data in array form
		float data[4];
	};

	//Normalizes quaternion to unit length, making it a valid orientation quaternion
	void normalize();

	//Multiples the quaternion by the given quaternion
	void operator*=(const quat& multiplier);

	//void rotateByVector(const vec3d vector);


	//Adds the given vector to this, scaled by the given amount. This is used to update the orientation quaternion by a rotation and time.
	void addScaledVector(const vec3d& vector, float scale);
};
