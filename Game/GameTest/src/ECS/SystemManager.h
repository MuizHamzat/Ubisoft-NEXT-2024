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
	std::shared_ptr<T> RegisterSystem();

	//Set the signature of a system
	template<typename T>
	void SetSignature(Signature signature);

	//Called when an entity is destroyed to notify each system
	void EntityDestroyed(Entity entity);

	//Called to notify each system that an entity's signature has changed. The entity will either be added or removed from the system's set of entities
	void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
	//Map from type string pointer to a system
	std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	//Map from type string pointer to a signature
	std::unordered_map<const char*, Signature> signatures{};
};
