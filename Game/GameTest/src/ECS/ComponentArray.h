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
	void InsertData(Entity entity, T component);

	//Used to remove a component from the array
	void RemoveData(Entity entity);

	//Used to get a reference to a component in the array
	T& GetData(Entity entity);

	void EntityDestroyed(Entity entity) override;

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
