#pragma once

#include "stdafx.h"

struct CameraComponent
{
	float fYaw = 0; // Camera yaw
	vec3d position; // Camera position
	vec3d lookDir; // Camera look direction
	vec3d target; // Camera target
	vec3d up; // Camera up direction
	mat4x4 cameraMatrix; // Camera matrix. This is used to convert world space into camera space
};
