#pragma once

#include "stdafx.h"
#include "Entity.h"
#include "Signature.h"
#include <queue>
#include <array>
#include <cassert>

class EntityManager //The entity manager class is in charge of distributing entity IDs and keeping record of which entities are in use and which ones aren't
{
public:
	EntityManager();

	//Create a new entity
	Entity CreateEntity();

	//Destroy an entity
	void DestroyEntity(Entity entity);

	//Set the signature of a given entity
	void SetSignature(Entity entity, Signature signature);

	//Get the signature of a given entity
	Signature GetSignature(Entity entity);

private:
	//Queue of unused entity IDs
	std::queue<Entity> availableEntities;

	//Array of signatures where the index corresponds to the entity ID
	std::array<Signature, MAX_ENTITIES> signatures{};

	//Total living entities - used to keep limits on how many entities can be created
	uint32_t livingEntityCount{};
};