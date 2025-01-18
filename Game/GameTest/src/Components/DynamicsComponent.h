#pragma once

#include "stdafx.h"
#include "src/ECS/Component.h"

struct DynamicsComponent //A component to give the entity it's applied to a mass and keep track of the net force applied on it
{
	vec3d velocity = { 0, 0, 0 };
	float mass = 1.0f;
	vec3d force = { 0, 0, 0 };
};
