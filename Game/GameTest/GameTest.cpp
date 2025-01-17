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
#include <algorithm>
#include <cmath>
#include "src/player.h"
#include "src/utility.h"
#include "app/app.h"
#include "src/Graphics/mesh.h"
#include "src/Graphics/Renderer.h"
#include "src/Math/matrix.h"
#include "src/Math/vec3d.h"

#include "src/ECS/Coordinator.h"
#include "src/Components/TransformComponent.h"
#include "src/Components/CameraComponent.h"
#include "src/Components/MeshComponent.h"
#include "src/Systems/RenderSystem.h"
#include "src/Systems/TransformSystem.h"
//------------------------------------------------------------------------
//Definitions
//Player player(400.0f, 400.0f);


Mesh meshCube;

float fTheta = 0.0f;
mat4x4 matRotX, matRotY, matRotZ; // Rotation matrices to perform rotations around the x, y, and z axes
mat4x4 matTrans;

Renderer renderer;

mat4x4 matProj; // Projection matrix
mat4x4 matWorld; // World matrix, which is the matrix that represents the position of the object in the world, and holds all the transformation information of the object
mat4x4 matCamera; // Camera matrix
mat4x4 matView; // View matrix

vec3d vCamera; // Camera position
vec3d vLookDir; // Camera look direction
vec3d vUp; // Camera up direction
vec3d vTarget; // Camera target

float fYaw;

std::vector<Mesh> meshes;

Coordinator gCoordinator;

std::shared_ptr<RenderSystem> renderSystem;
std::shared_ptr<TransformSystem> transformSystem;

std::vector<Entity> entities;
Entity ball;

//------------------------------------------------------------------------
void Init()
{
	//renderer.Init();

	//meshCube.LoadFromObjectFile("TestData/axis.obj");
	//meshes.push_back(meshCube);

	gCoordinator.Init();

	gCoordinator.RegisterComponent<TransformComponent>();
	gCoordinator.RegisterComponent<CameraComponent>();
	gCoordinator.RegisterComponent<MeshComponent>();

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

	//Create golf ball
	ball = gCoordinator.CreateEntity();
	entities.push_back(ball);

	TransformComponent ballTransform;
	ballTransform.position = vec3d{ 0, 0, 5 };
	ballTransform.rotation = vec3d{ 0, 0, 0 };
	ballTransform.scale = vec3d{ 1, 1, 1 };
	gCoordinator.AddComponent(ball, ballTransform);

	MeshComponent ballMesh;
	ballMesh.mesh.LoadFromObjectFile("TestData/golfBallLP.obj");
	ballMesh.color = Color{ 0, 0, 1 };
	gCoordinator.AddComponent(ball, ballMesh);

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
	holeMesh.color = Color{ 0, 1, 0 };
	gCoordinator.AddComponent(hole, holeMesh);
}
void Update(float deltaTime)
{
	float fTheta = 0.001f * deltaTime;
	//renderer.Update(deltaTime);
	renderSystem->Update(deltaTime);

	auto& ballTransform = gCoordinator.GetComponent<TransformComponent>(ball);
	ballTransform.rotation.x += fTheta;
	ballTransform.rotation.x = fmod(ballTransform.rotation.x, 2.0f * M_PI); // Keep rotation within 0-2pi radians

	for (auto& entity : entities)
	{
		auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
		transformComponent.isDirty = true;
	}

	transformSystem->Update(deltaTime);
}

void Render()
{
	//renderer.Render(meshes);
	renderSystem->Render();
}

void Shutdown()
{
	// No resources to clean up for line-drawn objects
}