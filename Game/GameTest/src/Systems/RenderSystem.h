#pragma once

#include "stdafx.h"
#include "src/ECS/Coordinator.h"

class RenderSystem : public System
{
public:
	void Init();
	void Update(float deltaTime);
	void Render();

private:
	Entity camera;

	mat4x4 matView;
	mat4x4 matProj;
	
	void MoveCamera(float deltaTime);
};