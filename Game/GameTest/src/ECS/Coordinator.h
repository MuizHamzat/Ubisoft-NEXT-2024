#pragma once

#include "stdafx.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

//The coordinator class is in charge of managing the ECS managers so they can all talk to each other
class Coordinator
{
public:
	void Init();

	//Entity methods

	Entity CreateEntity();

	void DestroyEntity(Entity entity);

	//Component Methods

	template<typename T>
	void RegisterComponent();

	template<typename T>
	void AddComponent(Entity entity, T component);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

	template<typename T>
	ComponentType GetComponentType();

	//System methods

	template<typename T>
	std::shared_ptr<T> RegisterSystem();

	template<typename T>
	void SetSystemSignature(Signature signature);

private:
	//A unique pointer is used to ensure that the memory for the manager is deallocated when the coordinator is destroyed
	std::unique_ptr<ComponentManager> m_ComponentManager;
	std::unique_ptr<EntityManager> m_EntityManager;
	std::unique_ptr<SystemManager> m_SystemManager;
};
