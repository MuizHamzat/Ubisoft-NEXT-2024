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
	void RegisterComponent();

	//Get the component array for a component type
	template<typename T>
	ComponentType GetComponentType();

	//Add a component to an entity
	template<typename T>
	void AddComponent(Entity entity, T component);

	//Remove a component from an entity
	template<typename T>
	void RemoveComponent(Entity entity);

	//Get a reference to a component on an entity
	template<typename T>
	T& GetComponent(Entity entity);

	//Called when an entity is destroyed to remove the entity's components
	void EntityDestroyed(Entity entity);

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
