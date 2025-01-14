#include "stdafx.h"

#include "SystemManager.h"

template<typename T>
std::shared_ptr<T> SystemManager::RegisterSystem()
{
	const char* typeName = typeid(T).name(); //Get the name of the type as a string
	assert(systems.find(typeName) == systems.end() && "System already registered."); //Make sure the system type is not already registered
	auto system = std::make_shared<T>(); //Create an instance of the system
	systems.insert({ typeName, system }); //Insert the system into the map
	return system;
}

template<typename T>
void SystemManager::SetSignature(Signature signature)
{
	const char* typeName = typeid(T).name(); //Get the name of the type as a string
	assert(systems.find(typeName) != systems.end() && "System used before registered."); //Make sure the system type is registered
	signatures.insert({ typeName, signature }); //Insert the signature into the map
}

void SystemManager::EntityDestroyed(Entity entity)
{
	//Erase a destroyed entity from all system lists
	//Entity signature changed, update the systems
	for (auto const& pair : systems)
	{
		auto const& system = pair.second;
		system->entities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
	//Notify each system that an entity's signature changed
	//Check each system if the entity should be added or removed
	for (auto const& pair : systems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = signatures[type];
		//Entity signature matches system signature - insert into set
		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->entities.insert(entity);
		}
		//Entity signature does not match system signature - erase from set
		else
		{
			system->entities.erase(entity);
		}
	}
}