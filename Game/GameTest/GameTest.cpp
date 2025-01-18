// GameTest.cpp
///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include <windows.h> 
#include <math.h>
#include <cmath>
#include "app/app.h"

#include "src/ECS/Coordinator.h"
#include "src/Components/TransformComponent.h"
#include "src/Components/CameraComponent.h"
#include "src/Components/MeshComponent.h"
#include "src/Components/DynamicsComponent.h"
#include "src/Systems/RenderSystem.h"
#include "src/Systems/TransformSystem.h"
#include "src/Systems/DynamicsSystem.h"
//------------------------------------------------------------------------
//Definitions

Mesh meshCube;

mat4x4 matRotX, matRotY, matRotZ; // Rotation matrices to perform rotations around the x, y, and z axes
mat4x4 matTrans;

mat4x4 matProj; // Projection matrix
mat4x4 matWorld; // World matrix, which is the matrix that represents the position of the object in the world, and holds all the transformation information of the object
mat4x4 matCamera, matView; // Camera and view matrices

vec3d vCamera; // Camera position
vec3d vLookDir; // Camera look direction
vec3d vUp; // Camera up direction
vec3d vTarget; // Camera target

Coordinator gCoordinator;

std::shared_ptr<RenderSystem> renderSystem;
std::shared_ptr<TransformSystem> transformSystem;
std::shared_ptr<DynamicsSystem> dynamicsSystem;

std::vector<Entity> entities;
Entity ball;

//------------------------------------------------------------------------
void Init()
{
	gCoordinator.Init();

	gCoordinator.RegisterComponent<TransformComponent>();
	gCoordinator.RegisterComponent<CameraComponent>();
	gCoordinator.RegisterComponent<MeshComponent>();
	gCoordinator.RegisterComponent<DynamicsComponent>();

	renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<TransformComponent>());
		signature.set(gCoordinator.GetComponentType<MeshComponent>());
		gCoordinator.SetSystemSignature<RenderSystem>(signature);
	}
	renderSystem->Init();

	transformSystem = gCoordinator.RegisterSystem<TransformSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<TransformComponent>());
		gCoordinator.SetSystemSignature<TransformSystem>(signature);
	}

	dynamicsSystem = gCoordinator.RegisterSystem<DynamicsSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<TransformComponent>());
		signature.set(gCoordinator.GetComponentType<DynamicsComponent>());
		gCoordinator.SetSystemSignature<DynamicsSystem>(signature);
	}

	//Create golf ball
	ball = gCoordinator.CreateEntity();
	entities.push_back(ball);

	TransformComponent ballTransform;
	ballTransform.position = vec3d{ 0, 5, 5 };
	ballTransform.rotation = vec3d{ 0, 0, 0 };
	ballTransform.scale = vec3d{ 1, 1, 1 };
	gCoordinator.AddComponent(ball, ballTransform);

	MeshComponent ballMesh;
	ballMesh.mesh.LoadFromObjectFile("TestData/golfBallLP.obj");
	ballMesh.color = Color{ 0, 0, 1 };
	gCoordinator.AddComponent(ball, ballMesh);

	DynamicsComponent ballDynamics;
	//ballDynamics.mass = 1.0f;
	gCoordinator.AddComponent(ball, ballDynamics);

	//Create hole
	Entity hole = gCoordinator.CreateEntity();
	entities.push_back(hole);

	TransformComponent holeTransform;
	holeTransform.position = vec3d{ 0, 0, 10 };
	holeTransform.rotation = vec3d{ M_PI/2, 0, 0 };
	holeTransform.scale = vec3d{ 3, 3, 3 };
	gCoordinator.AddComponent(hole, holeTransform);

	MeshComponent holeMesh;
	holeMesh.mesh.LoadFromObjectFile("TestData/donut.obj");
	holeMesh.color = Color{ 1, 1, 0 };
	gCoordinator.AddComponent(hole, holeMesh);
}
void Update(float deltaTime)
{
	float fTheta = 0.001f * deltaTime;

	auto& ballTransform = gCoordinator.GetComponent<TransformComponent>(ball);
	ballTransform.rotation.x += fTheta;
	ballTransform.rotation.x = fmod(ballTransform.rotation.x, 2.0f * M_PI); // Keep rotation within 0-2pi radians

	for (auto& entity : entities)
	{
		auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
		transformComponent.isDirty = true;
	}

	transformSystem->Update(deltaTime);
	dynamicsSystem->Update(deltaTime);
	renderSystem->Update(deltaTime);
}

void Render()
{
	renderSystem->Render();
}

void Shutdown()
{
	// No resources to clean up for line-drawn objects
}