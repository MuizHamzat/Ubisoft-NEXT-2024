// GameTest.cpp
///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <algorithm>
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

std::vector<Entity> entities(3);

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

	float counter = 0;
	//std::vector<std::string> meshFiles = { "TestData/donut.obj", "TestData/axis.obj", "TestData/teapot.obj" };

	for (auto& entity : entities)
	{
		entity = gCoordinator.CreateEntity();

		TransformComponent transformComponent;
		transformComponent.position = vec3d{ counter-1, 0, 3 };
		transformComponent.rotation = vec3d{ 1, 0, 0 };
		transformComponent.scale = vec3d{ 1, 1, 1 };
		gCoordinator.AddComponent(entity, transformComponent);

		MeshComponent meshComponent;
		meshComponent.mesh.LoadFromObjectFile("TestData/donut.obj");
		gCoordinator.AddComponent(entity, meshComponent);

		counter++;
	}

	//CameraComponent cameraComponent;
	//cameraComponent.position = vec3d{ 0,0,0 };
	//cameraComponent.lookDir = vec3d{ 0,0,-1 };
	//cameraComponent.target = VectorAdd(cameraComponent.position, cameraComponent.lookDir);
	//cameraComponent.up = vec3d{ 0,1,0 };
	////camera.cameraMatrix = MatrixPointAt(camera.pos, camera.target, camera.up)
	//cameraComponent.cameraMatrix = MatrixPointAt(cameraComponent.position, cameraComponent.target, cameraComponent.up);
	//gCoordinator.AddComponent(camera, cameraComponent);
}
void Update(float deltaTime)
{
	//renderer.Update(deltaTime);
	renderSystem->Update(deltaTime);

	for (auto& entity : entities)
	{
		auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
		//transformComponent.rotation.y += 0.001f * deltaTime;
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