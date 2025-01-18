#pragma once

#include "stdafx.h"
#include "src/ECS/Coordinator.h"
#include "src/Components/TransformComponent.h"
#include "src/Components/DynamicsComponent.h"

extern Coordinator gCoordinator;

class DynamicsSystem : public System
{
public:
	void Update(float deltaTime)
	{
		//Scale deltaTime so that a more relistic value can be used for gravity
		double scaledDeltaTime = (double)deltaTime * timeScale;
		
		for (auto const& entity : entities)
		{
			auto& dynamics = gCoordinator.GetComponent<DynamicsComponent>(entity);
			auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
			dynamics.force += gravity * dynamics.mass; //Apply gravity to the entity

			dynamics.velocity += dynamics.force / dynamics.mass * (float)scaledDeltaTime; //Update velocity
			transform.position += dynamics.velocity * (float)scaledDeltaTime; //Update position

			dynamics.force = { 0, 0, 0 }; //Reset force

		}
	}

private:
	vec3d gravity = { 0, -9.81f, 0 };
	double timeScale = 0.0001f;
};
