#include "stdafx.h"

#include "Renderer.h"
#include "App/app.h"

Renderer::Renderer(){}

void Renderer::Init()
{
	matWorld = MatrixMakeIdentity(); // Initialize the world matrix to the identity matrix
	// Camera's position is already intialized as {0, 0, 0} in the vec3d struct
	camera.lookDir = { 0, 0, -1 };
	camera.up = { 0, 1, 0 };
	camera.target = VectorAdd(camera.pos, camera.lookDir);

	camera.cameraMatrix = MatrixPointAt(camera.pos, camera.target, camera.up);
	matView = MatrixQuickInverse(camera.cameraMatrix);

	matProj = MatrixMakeProjection(90.0f, (float)APP_VIRTUAL_WIDTH / (float)APP_VIRTUAL_HEIGHT, 0.1f, 1000.0f);
}

void Renderer::Update(float deltaTime)
{
	MoveCamera(deltaTime);
	camera.cameraMatrix = MatrixPointAt(camera.pos, camera.target, camera.up);
	matView = MatrixQuickInverse(camera.cameraMatrix);
}

void Render(std::vector<Mesh>& meshes)
{
	std::vector<triangle> vecTrianglesToRaster; // Vector of triangles to rasterize
	
	for (auto& mesh : meshes)
	{
		for (auto& tri : mesh.tris)
		{

		}
	}
}

void Renderer::MoveCamera(float deltaTime)
{
	//Allow user to move camera
	//Up
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		camera.pos.y += 0.01f * deltaTime;
		//vLookDir.x += 0.01f * deltaTime;
	}
	//Down
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		camera.pos.y -= 0.01f * deltaTime;
	}
	//Left
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		camera.pos.x -= 0.01f * deltaTime;
	}
	//Right
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		camera.pos.x += 0.01f * deltaTime;
	}

	vec3d vForward = VectorMultiply(camera.lookDir, 0.01f * deltaTime);
	//Forward
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		camera.pos = VectorSubtract(camera.pos, vForward);
	}
	//Backward
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		camera.pos = VectorAdd(camera.pos, vForward);
	}

	// Rotate left
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		camera.fYaw -= 0.001f * deltaTime;
	}
	// Rotate right
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		camera.fYaw += 0.001f * deltaTime;
	}

	//Update the look direction
	camera.lookDir.x = sinf(camera.fYaw);
	camera.lookDir.z = -cosf(camera.fYaw);

	camera.target = VectorAdd(camera.pos, camera.lookDir);
}