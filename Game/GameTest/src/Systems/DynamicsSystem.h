#pragma once

#include "stdafx.h"
#include "src/ECS/Coordinator.h"
#include "src/Components/DynamicsComponent.h"

extern Coordinator gCoordinator;

class DynamicsSystem : public System
{
public:
	void Update(float deltaTime)
	{
		for (auto const& entity : entities)
		{
			auto& dynamics = gCoordinator.GetComponent<DynamicsComponent>(entity);
			dynamics.force += dynamics.mass * gravity;
		}
	}

private:
	vec3d gravity = { 0, -9.81f, 0 };
};
