#pragma once

#include "stdafx.h"
#include "src/ECS/Component.h"

struct DynamicsComponent //A component to give the entity it's applied to a mass and keep track of the net force applied on it
{
	vec3d velocity = { 0, 0, 0 };
	vec3d acceleration = { 0, 0, 0 };
	float mass = 1.0f;
	vec3d force = { 0, 0, 0 };
	float damping = 0.995;
	float inverseMass = 1/mass; //F = ma can be rearranged as a = 1/m * F. We can use the inverseMass instead of regular mass so that we can define objects with zero or inifite mass
};
