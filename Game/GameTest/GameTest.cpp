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

//------------------------------------------------------------------------
void Init()
{
	// Initial position of the object
	//player.SetPosition(400.0f, 400.0f);

	renderer.Init();

		//fYaw = 0.0f;

		//////Rotation matrices
		//matRotX = MatrixMakeRotationX(fTheta);
		//matRotY = MatrixMakeRotationY(fTheta);
		//matRotZ = MatrixMakeRotationZ(fTheta);

		//////Translation matrix
		//matTrans = MatrixMakeTranslation(0.0f, 0.0f, 30.0f);

		//////World matrix
		//matWorld = MatrixMakeIdentity(); // Initialize the world matrix to the identity matrix

	
		//vLookDir = { 1, 0, -1 }; // Look direction
		//vUp = { 0, 1, 0 }; // Up vector
		//vTarget = VectorAdd(vCamera, vLookDir); // Target vector

		//matCamera = MatrixPointAt(vCamera, vTarget, vUp); // Camera matrix
		//////Make view matrix from the camera matrix
		//matView = MatrixQuickInverse(matCamera);
	
		////// Projection Matrix
		////// The projection matrix is used to convert 3D coordinates into 2D coordinates. This is done by multiplying the 3D coordinates by the projection matrix.
		//matProj = MatrixMakeProjection(90.0f, (float)APP_VIRTUAL_HEIGHT / (float)APP_VIRTUAL_WIDTH, 0.1f, 1000.0f);

	meshCube.LoadFromObjectFile("TestData/axis.obj");
	meshes.push_back(meshCube);
}

void Update(float deltaTime)
{
	renderer.Update(deltaTime);
	
	////Allow user to move camera
	////Up
	//if (App::GetController().GetLeftThumbStickY() > 0.5f)
	//{
	//	vCamera.y += 0.01f * deltaTime;
	//	//vLookDir.x += 0.01f * deltaTime;
	//}
	//////Down
	//if (App::GetController().GetLeftThumbStickY() < -0.5f)
	//{
	//	vCamera.y -= 0.01f * deltaTime;
	//}
	//////Left
	//if (App::GetController().GetLeftThumbStickX() > 0.5f)
	//{
	//	vCamera.x -= 0.01f * deltaTime;
	//}
	//////Right
	//if (App::GetController().GetLeftThumbStickX() < -0.5f)
	//{
	//	vCamera.x += 0.01f * deltaTime;
	//}

	//vec3d vForward = VectorMultiply(vLookDir, 0.01f * deltaTime);
	//////Forward
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	//{
	//	vCamera = VectorSubtract(vCamera, vForward);
	//}
	//////Backward
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	//{
	//	vCamera = VectorAdd(vCamera, vForward);
	//}

	////// Rotate left
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	//{
	//	fYaw -= 0.001f * deltaTime;
	//}
	////// Rotate right
	//if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	//{
	//	fYaw += 0.001f * deltaTime;
	//}

	//////fTheta += 0.001f * deltaTime; // Rotate the object by 1 degree per frame

	//////Update the look direction
	//vLookDir.x = sinf(fYaw);
	//vLookDir.z = -cosf(fYaw);

	//////Update the rotation matrices with the new rotation
	//matRotX = MatrixMakeRotationX(fTheta);
	//matRotY = MatrixMakeRotationY(fTheta);
	//matRotZ = MatrixMakeRotationZ(fTheta);

	//matWorld = MatrixMultiplyMatrix(matRotZ, matRotX); // Multiply the world matrix by the Z and X rotation matrices, so the object rotates around the Z and X axes
	//matWorld = MatrixMultiplyMatrix(matWorld, matTrans); // Multiply the world matrix by the translation matrix, so the object is offset by the vector inputted to the translation matrix

	//vTarget = VectorAdd(vCamera, vLookDir); // Target vector

	//matCamera = MatrixPointAt(vCamera, vTarget, vUp); // Camera matrix
	//matView = MatrixQuickInverse(matCamera);
}

void Render()
{
	//   player.Draw();

	//   // Print player number
	   //std::vector<float> pos = player.GetPosition();
	   //std::string posStr = Utility::VectorToString(pos);
	//   App::Print(400.0f, 500.0f, posStr.c_str());

	/*std::vector<float> lookDirvec = { vLookDir.x, vLookDir.y, vLookDir.z };
	std::string lookDirStr = Utility::VectorToString(lookDirvec);
	App::Print(400.0f, 500.0f, lookDirStr.c_str());

	std::vector<float> Targetvec = { vTarget.x, vTarget.y, vTarget.z };
	std::string TargetStr = Utility::VectorToString(Targetvec);
	App::Print(400.0f, 450.0f, TargetStr.c_str());*/

	renderer.Render(meshes);
	
	//std::vector<triangle> vecTrianglesToRaster; // Vector of triangles to rasterize

	////Draw triangles
	//for (auto tri : meshCube.tris) // For each triangle in the mesh:
	//{
	//	triangle triProjected, triTransformed, triViewed; // Projected, transformed, and viewed triangles

	//	// Multiply the world matrix by each point of the triangle to get the transformed points
	//	triTransformed.p[0] = MatrixMultiplyVector(matWorld, tri.p[0]); 
	//	triTransformed.p[1] = MatrixMultiplyVector(matWorld, tri.p[1]);
	//	triTransformed.p[2] = MatrixMultiplyVector(matWorld, tri.p[2]);

	//	// Calculate the normal of the triangle
	//	vec3d normal, line1, line2;
	//	
	//	// Get the two lines of the triangle that extend from the first point of the triangle
	//	line1 = VectorSubtract(triTransformed.p[1], triTransformed.p[0]);
	//	line2 = VectorSubtract(triTransformed.p[2], triTransformed.p[0]);

	//	//The normal of the triangle is the cross prodict of these two lines
	//	normal = CrossProduct(line1, line2);
	//	
	//	//The normal must be normalized
	//	normal = Normalize(normal);

	//	//Make a ray from the triangle to camera
	//	vec3d vCameraRay = VectorSubtract(triTransformed.p[0], vCamera);

	//	//If the ray is aligned with the triangle's normal, then the triangle is visible to the camera, therefore draw it. Otherwise, don't draw it.
	//	if (DotProduct(normal, vCameraRay) < 0.0f)
	//	{
	//		//Convert world space into view space
	//		triViewed.p[0] = MatrixMultiplyVector(matView, triTransformed.p[0]);
	//		triViewed.p[1] = MatrixMultiplyVector(matView, triTransformed.p[1]);
	//		triViewed.p[2] = MatrixMultiplyVector(matView, triTransformed.p[2]);

	//		//Clip viewed triangle against near plane. This could form two additional triangles
	//		int nClippedTriangles = 0;
	//		triangle clipped[2];
	//		nClippedTriangles = TriangleClipAgainstPlane({ 0.0f, 0.0f, -0.1f }, { 0.0f, 0.0f, -1.0f }, triViewed, clipped[0], clipped[1]);

	//		for (int n = 0; n < nClippedTriangles; n++)
	//		{
	//			// Project triangles from 3D to 2D
	//			triProjected.p[0] = MatrixMultiplyVector(matProj, clipped[n].p[0]);
	//			triProjected.p[1] = MatrixMultiplyVector(matProj, clipped[n].p[1]);
	//			triProjected.p[2] = MatrixMultiplyVector(matProj, clipped[n].p[2]);

	//			// Scale into view, then normalize the projected triangle to the screen
	//			//We divide by the w component of the projected triangle to scale it into view
	//			//The difference between the z component and the w component is that the z component is the distance from the camera to the object, while the w component is the distance from the camera to the object divided by the field of view. This is used to scale the object into view.
	//			triProjected.p[0] = VectorDivide(triProjected.p[0], triProjected.p[0].w);
	//			triProjected.p[1] = VectorDivide(triProjected.p[1], triProjected.p[1].w);
	//			triProjected.p[2] = VectorDivide(triProjected.p[2], triProjected.p[2].w);

	//			//Offset the projected triangle to the center of the screen
	//			vec3d vOffsetView = { 1, 1, 0 };
	//			triProjected.p[0] = VectorAdd(triProjected.p[0], vOffsetView);
	//			triProjected.p[1] = VectorAdd(triProjected.p[1], vOffsetView);
	//			triProjected.p[2] = VectorAdd(triProjected.p[2], vOffsetView);
	//			//Scale the projected triangle to the screen
	//			triProjected.p[0].x *= 0.5f * (float)APP_VIRTUAL_WIDTH;
	//			triProjected.p[0].y *= 0.5f * (float)APP_VIRTUAL_HEIGHT;
	//			triProjected.p[1].x *= 0.5f * (float)APP_VIRTUAL_WIDTH;
	//			triProjected.p[1].y *= 0.5f * (float)APP_VIRTUAL_HEIGHT;
	//			triProjected.p[2].x *= 0.5f * (float)APP_VIRTUAL_WIDTH;
	//			triProjected.p[2].y *= 0.5f * (float)APP_VIRTUAL_HEIGHT;

	//			//Store triangle for sorting
	//			vecTrianglesToRaster.push_back(triProjected);
	//		}

	//	}
	//}

	////Sort triangles from back to front
	//std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2) //std::sort requires the following paramters: the beginning of the range to sort, the end of the range to sort, and a comparison function (lambda function in this case)
	//	{
	//		//Get the average z value of each triangle
	//		float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
	//		float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
	//		return z1 > z2;
	//	});

	//for (auto& triToRaster : vecTrianglesToRaster)
	//{
	//	//Clip triangles against all four screen edges
	//	triangle clipped[2];
	//	std::list<triangle> listTriangles; // List of triangles to rasterize
	//	listTriangles.push_back(triToRaster);
	//	int nNewTriangles = 1;

	//	for (int p = 0; p < 4; p++) // For each plane
	//	{
	//		int nTrisToAdd = 0;
	//		while (nNewTriangles > 0)
	//		{
	//			// Take triangle from front of queue
	//			triangle test = listTriangles.front();
	//			listTriangles.pop_front();
	//			nNewTriangles--;
	//			// Clip it against a plane. We only need to test each plane once
	//			switch (p)
	//			{
	//				//Case 0: clip against the top plane
	//			case 0: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
	//				//Case 1: clip against the bottom plane
	//			case 1: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, (float)APP_VIRTUAL_HEIGHT - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
	//				//Case 2: clip against the left plane
	//			case 2: nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
	//				//Case 3: clip against the right plane
	//			case 3: nTrisToAdd = TriangleClipAgainstPlane({ (float)APP_VIRTUAL_WIDTH - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
	//			}
	//			// Add any new triangles to the back of the queue
	//			for (int w = 0; w < nTrisToAdd; w++)
	//			{
	//				listTriangles.push_back(clipped[w]);
	//			}
	//		}
	//		nNewTriangles = listTriangles.size();
	//	}

	//	for (auto& t : listTriangles)
	//	{
	//		App::DrawLine(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y);
	//		App::DrawLine(t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
	//		App::DrawLine(t.p[2].x, t.p[2].y, t.p[0].x, t.p[0].y);
	//	}
	//}
}

void Shutdown()
{
	// No resources to clean up for line-drawn objects
}