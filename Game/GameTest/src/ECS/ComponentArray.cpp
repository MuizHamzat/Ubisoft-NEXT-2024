#include "stdafx.h"
#include "ComponentArray.h"


template<typename T>
void ComponentArray<T>::InsertData(Entity entity, T component)
{
	assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once."); //Make sure the entity doesn't already have this component
	//Add this component to the array
	size_t newIndex = size; //The index of the new component is the current size of the array
	entityToIndexMap[entity] = newIndex; //Map the entity to the index
	indexToEntityMap[newIndex] = entity; //Map the index to the entity
	componentArray[newIndex] = component; //Add the component to the array
	++size;
}

template<typename T>
void ComponentArray<T>::RemoveData(Entity entity)
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

template<typename T>
T& ComponentArray<T>::GetData(Entity entity)
{
	assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component."); //Make sure the entity has this component
	//Return a reference to the component
	return componentArray[entityToIndexMap[entity]];
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity)
{
	//If the entity is destroyed, remove the component
	if (entityToIndexMap.find(entity) != entityToIndexMap.end())
	{
		//Remove the component if the entity is destroyed
		RemoveData(entity);
	}
}