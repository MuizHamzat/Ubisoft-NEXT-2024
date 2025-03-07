#include "stdafx.h"

#include "EntityManager.h"

EntityManager::EntityManager()
{
	//Initialize a queue with all possible entity IDs
	for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
	{
		availableEntities.push(entity);
	}
}

Entity EntityManager::CreateEntity()
{
	assert(livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
	//Take an ID from the front of the queue
	Entity id = availableEntities.front();
	availableEntities.pop();
	++livingEntityCount;
	return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	//Invalidate the destroyed entity's signature
	signatures[entity].reset();
	//Put the destroyed ID at the back of the queue
	availableEntities.push(entity);
	--livingEntityCount;
}

void EntityManager::SetSignature(Entity entity, Signature signature)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");

	//Put this entity's signature into the array
	signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	//Get this entity's signature from the array
	return signatures[entity];
}