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
		double physicsDeltaTime = (double)deltaTime * timeScale;
		
		for (auto const& entity : entities)
		{
			auto& dynamics = gCoordinator.GetComponent<DynamicsComponent>(entity);
			auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
			dynamics.force += gravity * dynamics.mass; //Apply gravity to the entity

			dynamics.velocity += dynamics.force / dynamics.mass * (float)physicsDeltaTime; //Update velocity
			//Couldve also been dynamics.velocity += dynamics.acceleration * (float)physicsDeltaTime;
			transform.position += dynamics.velocity * (float)physicsDeltaTime; //Update position

			//Impose drag
			//dynamics.velocity *= powf(1 - dynamics.damping, (float)physicsDeltaTime); //Uses the physics equation v = v * (1 - damping)^t

			dynamics.force = { 0, 0, 0 }; //Reset force

		}
	}

private:
	vec3d gravity = { 0, -9.81f, 0 };
	double timeScale = 0.00075f;
};
