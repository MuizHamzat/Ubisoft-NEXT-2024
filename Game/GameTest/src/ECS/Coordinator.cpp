#include "stdafx.h"

#include "Coordinator.h"

void Coordinator::Init()
{
	//Initialize the ECS managers
	m_ComponentManager = std::make_unique<ComponentManager>();
	m_EntityManager = std::make_unique<EntityManager>();
	m_SystemManager = std::make_unique<SystemManager>();
}

//Entity methods
Entity Coordinator::CreateEntity()
{
	return m_EntityManager->CreateEntity();
}

void Coordinator::DestroyEntity(Entity entity)
{
	m_EntityManager->DestroyEntity(entity);
	m_ComponentManager->EntityDestroyed(entity);
	m_SystemManager->EntityDestroyed(entity);
}

//Component methods
template<typename T>
void Coordinator::RegisterComponent()
{
	m_ComponentManager->RegisterComponent<T>();
}

template<typename T>
void Coordinator::AddComponent(Entity entity, T component)
{
	m_ComponentManager->AddComponent<T>(entity, component);

	//Update the entity signature with the new component
	auto signature = m_EntityManager->GetSignature(entity);
	signature.set(m_ComponentManager->GetComponentType<T>(), true);
	m_EntityManager->SetSignature(entity, signature);

	m_SystemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
void Coordinator::RemoveComponent(Entity entity)
{
	m_ComponentManager->RemoveComponent<T>(entity);

	//Update the entity signature with the removed component
	auto signature = m_EntityManager->GetSignature(entity);
	signature.set(m_ComponentManager->GetComponentType<T>(), false);
	m_EntityManager->SetSignature(entity, signature);

	m_SystemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
T& Coordinator::GetComponent(Entity entity)
{
	return m_ComponentManager->GetComponent<T>(entity);
}

template<typename T>
ComponentType Coordinator::GetComponentType()
{
	return m_ComponentManager->GetComponentType<T>();
}

//System methods
template<typename T>
std::shared_ptr<T> Coordinator::RegisterSystem()
{
	return m_SystemManager->RegisterSystem<T>();
}

template<typename T>
void Coordinator::SetSystemSignature(Signature signature)
{
	m_SystemManager->SetSignature<T>(signature);
}