#pragma once

#include "stdafx.h"
#include "src/ECS/Component.h"

struct DynamicsComponent //A component to give the entity it's applied to a mass and keep track of the net force applied on it
{
	float mass;
	vec3d force;
};
