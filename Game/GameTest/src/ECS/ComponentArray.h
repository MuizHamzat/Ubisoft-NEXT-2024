#pragma once

#include "stdafx.h"
#include "Entity.h"
#include <unordered_map>
#include <array>


//IComponentArray is an interface class that is used to define the functions that a component array must implement
//We need this interface so that we can store different component arrays in the same container
class IComponentArray
{
public:
	virtual ~IComponentArray() = default; //Virtual destructor
	virtual void EntityDestroyed(Entity entity) = 0; //Pure virtual function to be overridden by derived classes
};


//The ComponentArray class is used to store components of a specific type
template<typename T>
class ComponentArray : public IComponentArray
{
public:
	//Used to insert a new component into the array
	void InsertData(Entity entity, T component)
	{
		assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once."); //Make sure the entity doesn't already have this component
		//Add this component to the array
		size_t newIndex = size; //The index of the new component is the current size of the array
		entityToIndexMap[entity] = newIndex; //Map the entity to the index
		indexToEntityMap[newIndex] = entity; //Map the index to the entity
		componentArray[newIndex] = component; //Add the component to the array
		++size;
	}

	//Used to remove a component from the array
	void RemoveData(Entity entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component."); //Make sure the entity has this component
		//Get the index of the component to remove
		size_t indexOfRemovedEntity = entityToIndexMap[entity];
		//Get the index of the last element in the array
		size_t indexOfLastElement = size - 1;
		//Move the last element to the index of the removed element
		componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];
		//Update the maps to point to the new index of the moved element
		Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
		entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;
		//Remove the entity and index mappings for the removed component
		entityToIndexMap.erase(entity);
		indexToEntityMap.erase(indexOfLastElement);
		--size;
	}

	//Used to get a reference to a component in the array
	T& GetData(Entity entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component."); //Make sure the entity has this component
		//Return a reference to the component
		return componentArray[entityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		//If the entity is destroyed, remove the component
		if (entityToIndexMap.find(entity) != entityToIndexMap.end())
		{
			//Remove the component if the entity is destroyed
			RemoveData(entity);
		}
	}

private:
	//The component array
	std::array<T, MAX_ENTITIES> componentArray;
	//Map of entities to indices
	std::unordered_map<Entity, size_t> entityToIndexMap;
	//Map of indices to entities
	std::unordered_map<size_t, Entity> indexToEntityMap;
	//Size of the array
	size_t size;
};
