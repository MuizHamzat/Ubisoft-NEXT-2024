#include "stdafx.h"

#include "RenderSystem.h"
#include "src/ECS/Coordinator.h"
#include "src/Components/TransformComponent.h"
#include "src/Components/CameraComponent.h"
#include "src/Components/MeshComponent.h"
#include "App/app.h"
#include <algorithm>

extern Coordinator gCoordinator;
//extern Entity camera;

void RenderSystem::Init()
{
	//INITIALIZING CAMERA

	camera = gCoordinator.CreateEntity();

	//Camera
	CameraComponent cameraComponent;
	cameraComponent.position = vec3d{ 0,0,0 };
	cameraComponent.lookDir = vec3d{ 0,0,-1 };
	cameraComponent.target = VectorAdd(cameraComponent.position, cameraComponent.lookDir);
	cameraComponent.up = vec3d{ 0,1,0 };
	//camera.cameraMatrix = MatrixPointAt(camera.pos, camera.target, camera.up)
	cameraComponent.cameraMatrix = MatrixPointAt(cameraComponent.position, cameraComponent.target, cameraComponent.up);
	gCoordinator.AddComponent(camera, cameraComponent);

	//Make view matrix from the camera matrix
	matView = MatrixQuickInverse(cameraComponent.cameraMatrix);

	matProj = MatrixMakeProjection(90.0f, (float)APP_VIRTUAL_HEIGHT / (float)APP_VIRTUAL_WIDTH, 0.1f, 1000.0f);
}

void RenderSystem::Update(float deltaTime)
{
	MoveCamera(deltaTime);
	//Update the view matrix
	auto& cameraComponent = gCoordinator.GetComponent<CameraComponent>(camera);
	cameraComponent.cameraMatrix = MatrixPointAt(cameraComponent.position, cameraComponent.target, cameraComponent.up);
	matView = MatrixQuickInverse(cameraComponent.cameraMatrix);
}

void RenderSystem::Render()
{
	auto& cameraComponent = gCoordinator.GetComponent<CameraComponent>(camera);
	std::vector<triangle> vecTrianglesToRaster; // Vector of triangles to rasterize
	
	for (auto& const entity : entities)
	{
		auto& transformComponent = gCoordinator.GetComponent<TransformComponent>(entity);
		auto& meshComponent = gCoordinator.GetComponent<MeshComponent>(entity);

		for (auto& tri : meshComponent.mesh.tris)
		{
			triangle triProjected, triTransformed, triViewed; // Projected, transformed, and viewed triangles

			//Transform the triangle
			triTransformed.p[0] = MatrixMultiplyVector(transformComponent.matWorld, tri.p[0]);
			triTransformed.p[1] = MatrixMultiplyVector(transformComponent.matWorld, tri.p[1]);
			triTransformed.p[2] = MatrixMultiplyVector(transformComponent.matWorld, tri.p[2]);

			//Calculate the normal of the triangle
			vec3d line1 = VectorSubtract(triTransformed.p[1], triTransformed.p[0]);
			vec3d line2 = VectorSubtract(triTransformed.p[2], triTransformed.p[0]);
			vec3d normal = CrossProduct(line1, line2);
			normal = Normalize(normal);

			//Make a ray from the triangle to the camera
			vec3d vCameraRay = VectorSubtract(triTransformed.p[0], cameraComponent.position);
			//If the ray is aligned with the triangle's normal, then the triangle is visible to the camera, therefore draw it. Otherwise, don't draw it.
			if (DotProduct(normal, vCameraRay) < 0.0f)
			{
				
				//Convert world space into view space
				triViewed.p[0] = MatrixMultiplyVector(matView, triTransformed.p[0]);
				triViewed.p[1] = MatrixMultiplyVector(matView, triTransformed.p[1]);
				triViewed.p[2] = MatrixMultiplyVector(matView, triTransformed.p[2]);

				//Clip viewed triangle against near plane. This could form two additional triangles
				int nClippedTriangles = 0;
				triangle clipped[2];
				nClippedTriangles = TriangleClipAgainstPlane({ 0.0f, 0.0f, -0.1f }, { 0.0f, 0.0f, -1.0f }, triViewed, clipped[0], clipped[1]);

				for (int n = 0; n < nClippedTriangles; n++)
				{
					// Project triangles from 3D to 2D
					triProjected.p[0] = MatrixMultiplyVector(matProj, clipped[n].p[0]);
					triProjected.p[1] = MatrixMultiplyVector(matProj, clipped[n].p[1]);
					triProjected.p[2] = MatrixMultiplyVector(matProj, clipped[n].p[2]);
					// Scale into view, then normalize the projected triangle to the screen
					triProjected.p[0] = VectorDivide(triProjected.p[0], triProjected.p[0].w);
					triProjected.p[1] = VectorDivide(triProjected.p[1], triProjected.p[1].w);
					triProjected.p[2] = VectorDivide(triProjected.p[2], triProjected.p[2].w);
					//Offset the projected triangle to the center of the screen
					vec3d vOffsetView = { 1, 1, 0 };
					triProjected.p[0] = VectorAdd(triProjected.p[0], vOffsetView);
					triProjected.p[1] = VectorAdd(triProjected.p[1], vOffsetView);
					triProjected.p[2] = VectorAdd(triProjected.p[2], vOffsetView);
					//Scale the projected triangle to the screen
					triProjected.p[0].x *= 0.5f * (float)APP_VIRTUAL_WIDTH;
					triProjected.p[0].y *= 0.5f * (float)APP_VIRTUAL_HEIGHT;
					triProjected.p[1].x *= 0.5f * (float)APP_VIRTUAL_WIDTH;
					triProjected.p[1].y *= 0.5f * (float)APP_VIRTUAL_HEIGHT;
					triProjected.p[2].x *= 0.5f * (float)APP_VIRTUAL_WIDTH;
					triProjected.p[2].y *= 0.5f * (float)APP_VIRTUAL_HEIGHT;
					//Store triangle for sorting
					vecTrianglesToRaster.push_back(triProjected);
				}
			}
		}
	}

	//Sort triangles from back to front
	std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
		{
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;
		});

	//Rasterize the triangles
	for (auto& triToRaster : vecTrianglesToRaster)
	{
		// Clip the triangle against all four screen edges, this could yield a bunch of triangles
		triangle clipped[2];
		std::list<triangle> listTriangles;
		listTriangles.push_back(triToRaster);
		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				// Take triangle from front of queue
				triangle test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;
				triangle clipped[2];
				// Clip it against a plane. We only need to test each point in the triangle against the near plane
				switch (p)
				{
					//Clip against the top plane
				case 0: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					//Clip against the bottom plane
				case 1: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, (float)APP_VIRTUAL_HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					//Clip against the left plane
				case 2: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					//Clip against the right plane
				case 3: nTrisToAdd = TriangleClipAgainstPlane({ (float)APP_VIRTUAL_WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				// Add any new triangles to the back of the queue
				for (int w = 0; w < nTrisToAdd; w++)
				{
					listTriangles.push_back(clipped[w]);
				}
			}
			// This is used to ensure that the loop runs for each plane
			nNewTriangles = listTriangles.size();
		}

		// Draw the clipped triangles
		for (auto& t : listTriangles)
		{
			App::DrawLine(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y);
			App::DrawLine(t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
			App::DrawLine(t.p[2].x, t.p[2].y, t.p[0].x, t.p[0].y);
		}
	}
}

void RenderSystem::MoveCamera(float deltaTime)
{
	auto& cameraComponent = gCoordinator.GetComponent<CameraComponent>(camera);
	
	//Allow user to move camera
			//Up
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		cameraComponent.position.y += 0.001f * deltaTime;
		//vLookDir.x += 0.01f * deltaTime;
	}
	//Down
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		cameraComponent.position.y -= 0.001f * deltaTime;
	}
	//Left
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		cameraComponent.position.x -= 0.001f * deltaTime;
	}
	//Right
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		cameraComponent.position.x += 0.001f * deltaTime;
	}

	vec3d vForward = VectorMultiply(cameraComponent.lookDir, 0.001f * deltaTime);
	//Forward
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		cameraComponent.position = VectorSubtract(cameraComponent.position, vForward);
	}
	//Backward
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		cameraComponent.position = VectorAdd(cameraComponent.position, vForward);
	}

	// Rotate left
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		cameraComponent.fYaw -= 0.001f * deltaTime;
	}
	// Rotate right
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		cameraComponent.fYaw += 0.001f * deltaTime;
	}

	//Update the look direction
	cameraComponent.lookDir.x = sinf(cameraComponent.fYaw);
	cameraComponent.lookDir.z = -cosf(cameraComponent.fYaw);

	cameraComponent.target = VectorAdd(cameraComponent.position, cameraComponent.lookDir);
}