#include "stdafx.h"

#include <algorithm>
#include "Renderer.h"
#include "App/app.h"

Renderer::Renderer(){}

void Renderer::Init()
{
	float fTheta = 0.0f;
	//Rotation matrices
	mat4x4 matRotX = MatrixMakeRotationX(fTheta);
	mat4x4 matRotY = MatrixMakeRotationY(fTheta);
	mat4x4 matRotZ = MatrixMakeRotationZ(fTheta);

	////Translation matrix
	mat4x4 matTrans = MatrixMakeTranslation(0.0f, 0.0f, 30.0f);
	
	//World matrix
	matWorld = MatrixMakeIdentity(); // Initialize the world matrix to the identity matrix
	// Camera's position is already intialized as {0, 0, 0} in the vec3d struct
	camera.lookDir = { 0, 0, -1 };
	camera.up = { 0, 1, 0 };
	camera.target = VectorAdd(camera.pos, camera.lookDir);

	camera.cameraMatrix = MatrixPointAt(camera.pos, camera.target, camera.up);
	//Make view matrix from the camera matrix
	matView = MatrixQuickInverse(camera.cameraMatrix);

	matProj = MatrixMakeProjection(90.0f, (float)APP_VIRTUAL_HEIGHT / (float)APP_VIRTUAL_WIDTH, 0.1f, 1000.0f);
}

void Renderer::Update(float deltaTime)
{
	MoveCamera(deltaTime);

	float fTheta = 0.0f;

	////Update the rotation matrices with the new rotation
	mat4x4 matRotX = MatrixMakeRotationX(fTheta);
	mat4x4 matRotY = MatrixMakeRotationY(fTheta);
	mat4x4 matRotZ = MatrixMakeRotationZ(fTheta);

	mat4x4 matTrans = MatrixMakeTranslation(0.0f, 0.0f, 30.0f);

	matWorld = MatrixMultiplyMatrix(matRotZ, matRotX); // Multiply the world matrix by the Z and X rotation matrices, so the object rotates around the Z and X axes
	matWorld = MatrixMultiplyMatrix(matWorld, matTrans); // Multiply the world matrix by the translation matrix, so the object is offset by the vector inputted to the translation matrix

	camera.cameraMatrix = MatrixPointAt(camera.pos, camera.target, camera.up);
	matView = MatrixQuickInverse(camera.cameraMatrix);
}

void Renderer::Render(std::vector<Mesh>& meshes)
{
	std::vector<triangle> vecTrianglesToRaster; // Vector of triangles to rasterize
	
	for (auto& mesh : meshes)
	{
		for (auto& tri : mesh.tris)
		{
			triangle triProjected, triTransformed, triViewed; // Projected, transformed, and viewed triangles

			//Transform the triangle
			triTransformed.p[0] = MatrixMultiplyVector(matWorld, tri.p[0]);
			triTransformed.p[1] = MatrixMultiplyVector(matWorld, tri.p[1]);
			triTransformed.p[2] = MatrixMultiplyVector(matWorld, tri.p[2]);

			//Calculate the normal of the triangle
			vec3d normal, line1, line2;
			line1 = VectorSubtract(triTransformed.p[1], triTransformed.p[0]);
			line2 = VectorSubtract(triTransformed.p[2], triTransformed.p[0]);
			normal = CrossProduct(line1, line2);
			normal = Normalize(normal);

			//Make a ray from the triangle to the camera
			vec3d vCameraRay = VectorSubtract(triTransformed.p[0], camera.pos);

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