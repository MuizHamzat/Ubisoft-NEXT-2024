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
#include <random>
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
Entity ball, hole, arrow;

bool xAim = false; 
bool yAim = false;
bool power = false;
bool shot = false;

float ballYaw, ballPitch, ballPower;

bool arrowForward = true;
bool arrowBackward = false;

bool dynamicsActive = false;

vec3d ringPosition;
vec3d ringNormal = { 0, 0, -1 };
float ringRadius;

vec3d prevBallPos;

int score = 0;

bool roundWon = false;

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
	ballTransform.position = vec3d{ 0, -0.5, 1 };
	prevBallPos = ballTransform.position;
	ballTransform.rotation = vec3d{ 0, 0, 0 };
	ballTransform.scale = vec3d{ 1, 1, 1 };
	gCoordinator.AddComponent(ball, ballTransform);

	MeshComponent ballMesh;
	ballMesh.mesh.LoadFromObjectFile("TestData/golfBallLP.obj");
	ballMesh.color = Color{ 1, 1, 1 };
	gCoordinator.AddComponent(ball, ballMesh);

	DynamicsComponent ballDynamics;
	gCoordinator.AddComponent(ball, ballDynamics);

	//Create hole
	hole = gCoordinator.CreateEntity();
	entities.push_back(hole);

	TransformComponent holeTransform;
	holeTransform.position = vec3d{ 0, 0, 8 };
	ringPosition = holeTransform.position;
	holeTransform.rotation = vec3d{ M_PI/2, 0, 0 };
	holeTransform.scale = vec3d{ 3, 3, 3 };
	ringRadius = 0.55f * holeTransform.scale.x;
	gCoordinator.AddComponent(hole, holeTransform);

	MeshComponent holeMesh;
	holeMesh.mesh.LoadFromObjectFile("TestData/donut.obj");
	holeMesh.color = Color{ 1, 1, 0 };
	gCoordinator.AddComponent(hole, holeMesh);

	//Create arrow
	arrow = gCoordinator.CreateEntity();
	entities.push_back(arrow);

	TransformComponent arrowTransform;
	arrowTransform.position = vec3d{ 0, -0.5, 1.2 };
	//arrowTransform.rotation = vec3d{ 0, -M_PI/2, 0 };
	arrowTransform.scale = vec3d{ 1, 1, 1 };
	gCoordinator.AddComponent(arrow, arrowTransform);

	MeshComponent arrowMesh;
	arrowMesh.mesh.LoadFromObjectFile("TestData/arrow.obj");
	arrowMesh.color = Color{ 1, 0, 0 };
	gCoordinator.AddComponent(arrow, arrowMesh);
}

void Update(float deltaTime)
{
#pragma region Aiming
	float fTheta = 0.001f * deltaTime;
	float scaleFactorIncrease = 1 + 0.002f * deltaTime;
	float scaleFactorDecrease = 1 - 0.002f * deltaTime;

	auto& ballTransform = gCoordinator.GetComponent<TransformComponent>(ball);
	auto& arrowTransform = gCoordinator.GetComponent<TransformComponent>(arrow);

	if (!xAim && !yAim && !power && !shot)
	{
		xAim = true;
	}

	if (xAim)
	{
		// Calculate the new position of the arrow
		float radius = 0.0f; // Distance from the ball to the arrow
		arrowTransform.position.x = ballTransform.position.x + radius * cos(fTheta);
		arrowTransform.position.z = ballTransform.position.z + radius * sin(fTheta);
		arrowTransform.position.y = ballTransform.position.y;
		
		//Change the rotation so that the arrow bounces between a third of a circle
		if (arrowTransform.rotation.y > 1.05f)
		{
			arrowBackward = true;
			arrowForward = false;
		}
		else if (arrowTransform.rotation.y < -1.05f)
		{
			arrowBackward = false;
			arrowForward = true;
		}

		if (arrowForward)
		{
			arrowTransform.rotation.y += fTheta;
		}
		else if (arrowBackward)
		{
			arrowTransform.rotation.y -= fTheta;
		}

		//If the player hits A, switch to pitch
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
		{
			ballYaw = -arrowTransform.rotation.y;
			xAim = false;
			yAim = true;
		}
	}

	if (yAim)
	{
		// Calculate the new position of the arrow
		float radius = 0.0f; // Distance from the ball to the arrow
		arrowTransform.position.x = ballTransform.position.x;
		arrowTransform.position.z = ballTransform.position.z + radius * sin(fTheta);
		arrowTransform.position.y = ballTransform.position.y + radius * cos(fTheta);

		//Change the rotation so that the arrow bounces between a third of a circle
		if (arrowTransform.rotation.x < -1.0f)
		{
			arrowBackward = false;
			arrowForward = true;
		}
		else if (arrowTransform.rotation.x > 0.0f)
		{
			arrowBackward = true;
			arrowForward = false;
		}

		if (arrowForward)
		{
			arrowTransform.rotation.x += fTheta;
		}
		else if (arrowBackward)
		{
			arrowTransform.rotation.x -= fTheta;
		}

		//If the player hits B, switch to power
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
		{
			ballPitch = -arrowTransform.rotation.x;
			yAim = false;
			power = true;
		}
	}

	if (power)
	{
		//Change the rotation so that the arrow bounces between a third of a circle
		if (arrowTransform.scale.z > 3.0f)
		{
			arrowBackward = true;
			arrowForward = false;
		}
		else if (arrowTransform.scale.z < 0.4f)
		{
			arrowBackward = false;
			arrowForward = true;
		}

		if (arrowForward)
		{
			arrowTransform.scale.z *= scaleFactorIncrease;
		}
		else if (arrowBackward)
		{
			arrowTransform.scale.z *= scaleFactorDecrease;
		}

		//If the player hits X, switch to the ball being shot
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_X, true))
		{
			ballPower = arrowTransform.scale.z;
			power = false;
			shot = true;
		}
	}
#pragma endregion

	//If ball is in shot mode, turn on dynamics, and apply an impulse to the ball based on it's pitch, yaw, and power
	if (shot)
	{
		//Make the arrow invisible
		arrowTransform.scale = vec3d{ 0, 0, 0 };
		
		if (!dynamicsActive)
		{
			dynamicsActive = true;
		}
		auto& ballDynamics = gCoordinator.GetComponent<DynamicsComponent>(ball);
		//ballDynamics.force = { ballPower * 15 * cos(ballPitch) * sin(ballYaw), ballPower * 15 * sin(ballPitch), ballPower * 15 * cos(ballPitch) * cos(ballYaw) };
		ballDynamics.velocity += vec3d{ ballPower * 10 * cos(ballPitch) * sin(ballYaw), 
			ballPower * 10 * sin(ballPitch), 
			ballPower * 10 * cos(ballPitch) * cos(ballYaw) } / ballDynamics.mass;
		shot = false;
	}

	for (auto& entity : entities)
	{
		auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
		transformComponent.isDirty = true;
	}

	if (dynamicsActive)
	{
		//Compute the distance between the ball and the ring
		float prevDist = DotProduct(prevBallPos - ringPosition, ringNormal);
		float currDist = DotProduct(gCoordinator.GetComponent<TransformComponent>(ball).position - ringPosition, ringNormal);

		//Check if the ball has passed through the ring's plane
		bool crossedPlane = (prevDist > 0 && currDist < 0);

		//Check if the ball is within the ring's radius
		float prevXYDist = Magnitude(prevBallPos - ringPosition);
		float currXYDist = Magnitude(gCoordinator.GetComponent<TransformComponent>(ball).position - ringPosition);

		bool withinRing = (currXYDist <= ringRadius);

		//Only count if the ball actually passed through the ring
		if (crossedPlane && withinRing)
		{
			score++;
			//Chang the ring's color to green
			auto& holeMesh = gCoordinator.GetComponent<MeshComponent>(hole);
			holeMesh.color = Color{ 0, 1, 0 };
			roundWon = true;
		}

		//Update the previous ball position
		prevBallPos = gCoordinator.GetComponent<TransformComponent>(ball).position;
	}

	//Once the ball's y position reaches a certain point, reset the ball, arrow, and ring
	if (gCoordinator.GetComponent<TransformComponent>(ball).position.y < -5)
	{
		auto& ballTransform = gCoordinator.GetComponent<TransformComponent>(ball);
		ballTransform.position = vec3d{ 0, -0.5, 1 };
		ballTransform.rotation = vec3d{ 0, 0, 0 };
		ballTransform.scale = vec3d{ 1, 1, 1 };
		//Reset the ball's velocity
		auto& ballDynamics = gCoordinator.GetComponent<DynamicsComponent>(ball);
		ballDynamics.velocity = { 0, 0, 0 };

		auto& arrowTransform = gCoordinator.GetComponent<TransformComponent>(arrow);
		arrowTransform.position = vec3d{ 0, -0.5, 1.2 };
		arrowTransform.rotation = vec3d{ 0, 0, 0 };
		arrowTransform.scale = vec3d{ 1, 1, 1 };

		auto& holeTransform = gCoordinator.GetComponent<TransformComponent>(hole);
		//Give the hole a random position, with x values between -5 and 5, y values between 0 and 4, and z values between 5 and 15
        holeTransform.position = vec3d{ static_cast<float>((rand() % 10) - 5), static_cast<float>(rand() % 4), static_cast<float>((rand() % 10) + 5) };
		ringPosition = holeTransform.position;
		ringNormal = { 0, 0, -1 };
		ringRadius = 0.55f * holeTransform.scale.x;
		//Change the ring's color back to yellow
		auto& holeMesh = gCoordinator.GetComponent<MeshComponent>(hole);
		holeMesh.color = Color{ 1, 1, 0 };
		dynamicsActive = false;
	}
	

	transformSystem->Update(deltaTime);
	if (dynamicsActive) dynamicsSystem->Update(deltaTime);
	renderSystem->Update(deltaTime);
}

void Render()
{
	renderSystem->Render();

	//Print the score
	App::Print(100, 500, std::to_string(score).c_str());
}

void Shutdown()
{
	// No resources to clean up for line-drawn objects
}