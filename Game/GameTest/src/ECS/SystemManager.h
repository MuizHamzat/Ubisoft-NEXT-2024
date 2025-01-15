#pragma once

#include "stdafx.h"
#include "System.h"
#include "Signature.h"
#include "Entity.h"
#include <unordered_map>


//The system manager is in charge of maintaining a record of registered systems and their signatures.
//It is also responsible for updating systems when entities are created, updated, or destroyed.
class SystemManager
{
public:
	//Register a system to the scene
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name(); //Get the name of the type as a string
		assert(systems.find(typeName) == systems.end() && "System already registered."); //Make sure the system type is not already registered
		auto system = std::make_shared<T>(); //Create an instance of the system
		systems.insert({ typeName, system }); //Insert the system into the map
		return system;
	}

	//Set the signature of a system
	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name(); //Get the name of the type as a string
		assert(systems.find(typeName) != systems.end() && "System used before registered."); //Make sure the system type is registered
		signatures.insert({ typeName, signature }); //Insert the signature into the map
	}

	//Called when an entity is destroyed to notify each system
	void EntityDestroyed(Entity entity)
	{
		//Erase a destroyed entity from all system lists
		//Entity signature changed, update the systems
		for (auto const& pair : systems)
		{
			auto const& system = pair.second;
			system->entities.erase(entity);
		}
	}

	//Called to notify each system that an entity's signature has changed. The entity will either be added or removed from the system's set of entities
	void EntitySignatureChanged(Entity entity, Signature entitySignature)
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

private:
	//Map from type string pointer to a system
	std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	//Map from type string pointer to a signature
	std::unordered_map<const char*, Signature> signatures{};
};
