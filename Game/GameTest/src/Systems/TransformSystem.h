#pragma once

#include "stdafx.h"
#include "src/ECS/System.h"
#include "src/ECS/Coordinator.h"
#include "src/Components/TransformComponent.h"

extern Coordinator gCoordinator;

class TransformSystem : public System
{
public:
	void Update(float deltaTime)
	{
		for (auto const& entity : entities)
		{
			auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
			transform.UpdateWorldMatrix();
		}
	}
};
