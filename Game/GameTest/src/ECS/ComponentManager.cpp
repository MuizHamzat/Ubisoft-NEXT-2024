#include "stdafx.h"
#include "ComponentManager.h"

template <typename T>
void ComponentManager::RegisterComponent()
{
	const char* typeName = typeid(T).name(); //Get the name of the type as a string
	assert(componentTypes.find(typeName) == componentTypes.end() && "Registering component type more than once."); //Make sure the component type is not already registered
	componentTypes.insert({ typeName, nextComponentType }); //Insert the component type into the map
	componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() }); //Insert the component array into the map
	++nextComponentType;
}

template<typename T>
ComponentType ComponentManager::GetComponentType()
{
	const char* typeName = typeid(T).name(); //Get the name of the type as a string

	assert(ComponentTypes.find(typeName) != ComponentTypes.end() && "Component not registered before use."); //Make sure the component type is registered
	return ComponentTypes[typeName]; //Return the component type (used for creating signatures)
}

template<typename T>
void ComponentManager::AddComponent(Entity entity, T component)
{
	//Add a component to the array for an entity
	GetComponentArray<T>()->InsertData(entity, component);
}


template<typename T>
void ComponentManager::RemoveComponent(Entity entity)
{
	GetComponentArray<T>->RemoveData(entity);
}

template<typename T>
T& ComponentManager::GetComponent(Entity entity)
{
	//Get a reference to a component from the array for an entity
	return GetComponentArray<T>()->GetData(entity);
}

void ComponentManager::EntityDestroyed(Entity entity)
{
	//Notify each component array that an entity has been destroyed
	//If it has a component for that entity, it will remove it
	for (auto const& pair : componentArrays)
	{
		auto const& component = pair.second;
		component->EntityDestroyed(entity);
	}
}