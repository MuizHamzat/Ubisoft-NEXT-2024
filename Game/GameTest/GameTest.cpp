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
#include "src/Math/matrix.h"
#include "src/Math/vec3d.h"
//------------------------------------------------------------------------
//Definitions
//Player player(400.0f, 400.0f);

float fTheta = 0.0f;
mat4x4 matRotZ, matRotX; // Rotation matrices to perform rotations around the Z and X axes
Mesh meshCube;
mat4x4 matProj; // Projection matrix

vec3d vCamera; // Camera position

//------------------------------------------------------------------------
void Init()
{
	// Initial position of the object
	//player.SetPosition(400.0f, 400.0f);

	//Hard-code in the rotation matrices

	// Rotation Z matrix
	matRotZ.m[0][0] = cosf(fTheta);
	matRotZ.m[0][1] = sinf(fTheta);
	matRotZ.m[1][0] = -sinf(fTheta);
	matRotZ.m[1][1] = cosf(fTheta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	// Rotation X matrix
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(fTheta * 0.5f);
	matRotX.m[1][2] = sinf(fTheta * 0.5f);
	matRotX.m[2][1] = -sinf(fTheta * 0.5f);
	matRotX.m[2][2] = cosf(fTheta * 0.5f);
	matRotX.m[3][3] = 1;

	//meshCube.tris = {
	//	// South (-z face)
	//	{ 0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f },
	//	{ 0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f },

	//	// East (+x face)
	//	{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f },
	//	{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 1.0f },

	//	// North (+z face)
	//	{ 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 1.0f },
	//	{ 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f },

	//	// West (-x face)
	//	{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f },
	//	{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f },

	//	// Top (+y face)
	//	{ 0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f },
	//	{ 0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 0.0f },

	//	// Bottom (-y face)
	//	{ 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f },
	//	{ 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f }

	//};

	meshCube.LoadFromObjectFile("TestData/donut.obj");

	// Projection Matrix
	// The projection matrix is used to convert 3D coordinates into 2D coordinates. This is done by multiplying the 3D coordinates by the projection matrix.

	float fNear = 0.1f; // Near clipping plane
	float fFar = 1000.0f; // Far clipping plane
	float fFov = 90.0f; // Field of view (90 degrees)
	float fAspectRatio = (float)APP_VIRTUAL_HEIGHT / (float)APP_VIRTUAL_WIDTH;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f); // Convert field of view from degrees to radians

	// Set the projection matrix (you could think of this as the projection matrix's definition. Any indexes that aren't declared here were initialized as 0).
	matProj.m[0][0] = fAspectRatio * fFovRad;
	matProj.m[1][1] = fFovRad;
	matProj.m[2][2] = fFar / (fFar - fNear);
	matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj.m[2][3] = 1.0f;
	matProj.m[3][3] = 0.0f;
}

void Update(float deltaTime)
{
	/*if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		player.x += 1;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		player.x -= 1;
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		player.y += 1;
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		player.y -= 1;
	}*/

	fTheta += 0.001f * deltaTime; // Rotate the cube by 1 degree per frame

	//Update the rotation matrices with the new rotation
	// Rotation Z matrix
	matRotZ.m[0][0] = cosf(fTheta);
	matRotZ.m[0][1] = sinf(fTheta);
	matRotZ.m[1][0] = -sinf(fTheta);
	matRotZ.m[1][1] = cosf(fTheta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	// Rotation X matrix
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(fTheta * 0.5f);
	matRotX.m[1][2] = sinf(fTheta * 0.5f);
	matRotX.m[2][1] = -sinf(fTheta * 0.5f);
	matRotX.m[2][2] = cosf(fTheta * 0.5f);
	matRotX.m[3][3] = 1;
}

void Render()
{
	//   player.Draw();

	//   // Print player number
	   //std::vector<float> pos = player.GetPosition();
	   //std::string posStr = Utility::VectorToString(pos);
	//   App::Print(400.0f, 500.0f, posStr.c_str());

	std::vector<triangle> vecTrianglesToRaster; // Vector of triangles to rasterize

	//Draw triangles
	for (auto tri : meshCube.tris) // For each triangle in the mesh:
	{
		triangle triProjected, triTranslated, triRotatedZ, triRotatedZX; // Transformed triangles to be projected, translated, and rotated

		// Rotate the triangle around the Z axis
		MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
		MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
		MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

		// Rotate the triangle around the X axis
		MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
		MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
		MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

		// Translate the triangle so it's not centered at the origin, which is where the camera is
		triTranslated = triRotatedZX;
		//Offset the triangle by 3 units
		triTranslated.p[0].z = triRotatedZX.p[0].z + 1.0f;
		triTranslated.p[1].z = triRotatedZX.p[1].z + 1.0f;
		triTranslated.p[2].z = triRotatedZX.p[2].z + 1.0f;

		// Calculate the normal of the triangle
		vec3d normal, line1, line2;
		line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
		line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
		line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

		line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
		line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
		line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

		//normal is the cross product of line1 and line2
		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;

		//Normalize the normal
		float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z); // Length of the normal found using Pythagorean theorem
		normal.x /= l; normal.y /= l; normal.z /= l; // Normalize the normal

		//if (normal.z < 0) // If the normal line of the triangle's z-component is less than 0, the triangle is facing the camera, so we should draw it. Otherwise, it's facing away from the camera and we shouldn't draw it.
		// If the dot product of the normal and the vector from the camera to the triangle is less than 0, the triangle is facing the camera
		if (normal.x * (triTranslated.p[0].x - vCamera.x) +
			normal.y * (triTranslated.p[0].y - vCamera.y) +
			normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
		{

			// Project triangles from 3D to 2D
			MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
			MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
			MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

			//Scale into view
			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

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

	//Sort triangles from back to front
	std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2) //std::sort requires the following paramters: the beginning of the range to sort, the end of the range to sort, and a comparison function (lambda function in this case)
		{
			//Get the average z value of each triangle
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;
		});

	for (auto& triProjected : vecTrianglesToRaster)
	{
		App::DrawLine(triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y);
		App::DrawLine(triProjected.p[1].x, triProjected.p[1].y, triProjected.p[2].x, triProjected.p[2].y);
		App::DrawLine(triProjected.p[2].x, triProjected.p[2].y, triProjected.p[0].x, triProjected.p[0].y);
	}
}

void Shutdown()
{
	// No resources to clean up for line-drawn objects
}