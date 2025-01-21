#pragma once

#include "stdafx.h"
#include "src/ECS/Coordinator.h"
#include "src/Components/DynamicsComponent.h"
#include "src/Components/TransformComponent.h"

extern Coordinator gCoordinator;

struct Contact
{
	Entity entitiesColliding[2];
	DynamicsComponent& dynamics1 = gCoordinator.GetComponent<DynamicsComponent>(entitiesColliding[0]);
	TransformComponent& transform1 = gCoordinator.GetComponent<TransformComponent>(entitiesColliding[0]);
	float restitution;
	vec3d contactNormal;
	float penetration;

public:
	void resolve(float deltaTime)
	{
		resolveVelocity(deltaTime);
		resolveInterpenetration(deltaTime);
	}

	float calculateSeparatingVelocity()
	{
		vec3d relativeVelocity = dynamics1.velocity;
		if (entitiesColliding[1] != NULL)
		{
			auto& dynamics2 = gCoordinator.GetComponent<DynamicsComponent>(entitiesColliding[1]);
			relativeVelocity -= dynamics2.velocity;
		}
		return DotProduct(relativeVelocity, contactNormal);
	}

private:
	void resolveVelocity(float deltaTime)
	{
		//Find the velocity in the direction of the contact
		float separatingVelocity = calculateSeparatingVelocity();

		//Check if it needs to be resolved
		if (separatingVelocity > 0)
		{
			//The contact is either separating or stationary - there's no impulse required
			return;
		}

		//Calculate the new separating velocity
		float newSepVelocity = -separatingVelocity * restitution;

		//Check the velocity build-up due to acceleration only
		vec3d accCausedVelocity = dynamics1.acceleration;
		if (entitiesColliding[1] != NULL)
		{
			auto& dynamics2 = gCoordinator.GetComponent<DynamicsComponent>(entitiesColliding[1]);
			accCausedVelocity -= dynamics2.acceleration;
		}
		float accCausedSepVelocity = DotProduct(accCausedVelocity, contactNormal) * deltaTime;

		//If we've got a closing velocity due to acceleration build-up, remove it from the new separating velocity
		if (accCausedSepVelocity < 0)
		{
			newSepVelocity += restitution * accCausedSepVelocity;
			//Make sure we haven't removed more than there was to remove
			if (newSepVelocity < 0) newSepVelocity = 0;
		}

		float deltaVelocity = newSepVelocity - separatingVelocity;

		//We apply the change in velocity to each object in proportion to their inverse mass (i.e. those with lower inverse mass [higher actual mass] get less change in velocity)
		float totalInverseMass = dynamics1.inverseMass;

		if (entitiesColliding[1] != NULL)
		{
			auto& dynamics2 = gCoordinator.GetComponent<DynamicsComponent>(entitiesColliding[1]);
			totalInverseMass += dynamics2.inverseMass;
		}

		//If all entites have infinite mass, then impulses have no effect
		if (totalInverseMass <= 0) return;

		//Calculate the impulse to apply
		float impulse = deltaVelocity / totalInverseMass;

		//Find the amount of impulse per unit of inverse mass
		vec3d impulsePerIMass = contactNormal * impulse;

		//Apply impulses: they are applied in the direction of the contact, and are proportional to the inverse mass
		dynamics1.velocity += impulsePerIMass * dynamics1.inverseMass;
		if (entitiesColliding[1] != NULL)
		{
			//The second entity goes in the opposite direction
			auto& dynamics2 = gCoordinator.GetComponent<DynamicsComponent>(entitiesColliding[1]);
			dynamics2.velocity += impulsePerIMass * -dynamics2.inverseMass;
		}
	}

	void resolveInterpenetration(float deltaTime)
	{
		//If there is no penetration, skip this step
		if (penetration <= 0) return;

		//The movement of each object is based on their inverse mass
		float totalInverseMass = dynamics1.inverseMass;
		if (entitiesColliding[1] != NULL)
		{
			auto& dynamics2 = gCoordinator.GetComponent<DynamicsComponent>(entitiesColliding[1]);
			totalInverseMass += dynamics2.inverseMass;
		}

		//If all entites have infinite mass, then we can't move them
		if (totalInverseMass <= 0) return;

		//Find the amount of penetration resolution per unit of inverse mass
		vec3d movePerIMass = contactNormal * (-penetration / totalInverseMass);

		//Apply the penetration resolution
		transform1.position += movePerIMass * dynamics1.inverseMass;
		if (entitiesColliding[1] != NULL)
		{
			auto& dynamics2 = gCoordinator.GetComponent<DynamicsComponent>(entitiesColliding[1]);
			auto& transform2 = gCoordinator.GetComponent<TransformComponent>(entitiesColliding[1]);
			transform2.position += movePerIMass * -dynamics2.inverseMass;
		}
	}
};
