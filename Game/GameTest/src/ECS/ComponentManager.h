#pragma once

#include "stdafx.h"
#include "Component.h"
#include "ComponentArray.h"
#include <unordered_map>

//An unordered map is a hash table that is used to store key-value pairs

//The component manager is in charge of talking to all of the different ComponentArrays when a component needs to be added or removed

class ComponentManager
{
public:
	//Register a component type to the scene
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name(); //Get the name of the type as a string
		assert(componentTypes.find(typeName) == componentTypes.end() && "Registering component type more than once."); //Make sure the component type is not already registered
		componentTypes.insert({ typeName, nextComponentType }); //Insert the component type into the map
		componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() }); //Insert the component array into the map
		++nextComponentType;
	}

	//Get the component array for a component type
	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name(); //Get the name of the type as a string

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use."); //Make sure the component type is registered
		return componentTypes[typeName]; //Return the component type (used for creating signatures)
	}

	//Add a component to an entity
	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		//Add a component to the array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	//Remove a component from an entity
	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>->RemoveData(entity);
	}

	//Get a reference to a component on an entity
	template<typename T>
	T& GetComponent(Entity entity)
	{
		//Get a reference to a component from the array for an entity
		return GetComponentArray<T>()->GetData(entity);
	}

	//Called when an entity is destroyed to remove the entity's components
	void EntityDestroyed(Entity entity)
	{
		//Notify each component array that an entity has been destroyed
		//If it has a component for that entity, it will remove it
		for (auto const& pair : componentArrays)
		{
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

private:
	//Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> componentTypes{};
	//Map from type string pointer to a component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};
	//The component type to be assigned to the next registered component (starting at 0)
	ComponentType nextComponentType{};

	//Helper function to get the component array for a type
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name(); //Get the name of the type as a string
		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use."); //Make sure the component type is registered
		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]); //Return the component array
	}
};
