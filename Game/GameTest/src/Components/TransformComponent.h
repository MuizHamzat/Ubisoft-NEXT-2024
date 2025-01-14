#pragma once

#include "stdafx.h"

#include "src/ECS/Component.h"
#include "src/Math/vec3d.h"

struct TransformComponent
{
	//The world matrix for the entity is calculated in a system using these variables
	vec3d position; //Position of the entity
	vec3d rotation; //Rotation of the entity
	vec3d scale; //Scale of the entity
};
