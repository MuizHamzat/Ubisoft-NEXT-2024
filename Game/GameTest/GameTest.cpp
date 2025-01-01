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


Mesh meshCube;

float fTheta = 0.0f;
mat4x4 matRotX, matRotY, matRotZ; // Rotation matrices to perform rotations around the x, y, and z axes
mat4x4 matTrans;
mat4x4 matProj; // Projection matrix
mat4x4 matWorld; // World matrix, which is the matrix that represents the position of the object in the world, and holds all the transformation information of the object

vec3d vCamera; // Camera position

//------------------------------------------------------------------------
void Init()
{
	// Initial position of the object
	//player.SetPosition(400.0f, 400.0f);

	//Rotation matrices
	matRotX = MatrixMakeRotationX(fTheta);
	matRotY = MatrixMakeRotationY(fTheta);
	matRotZ = MatrixMakeRotationZ(fTheta);

	//Translation matrix
	matTrans = MatrixMakeTranslation(0.0f, 0.0f, 150.0f);

	//World matrix
	matWorld = MatrixMakeIdentity(); // Initialize the world matrix to the identity matrix
	

	meshCube.LoadFromObjectFile("TestData/teapot.obj");

	// Projection Matrix
	// The projection matrix is used to convert 3D coordinates into 2D coordinates. This is done by multiplying the 3D coordinates by the projection matrix.

	matProj = MatrixMakeProjection(90.0f, (float)APP_VIRTUAL_HEIGHT / (float)APP_VIRTUAL_WIDTH, 0.1f, 1000.0f);
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
	matRotX = MatrixMakeRotationX(fTheta);
	matRotY = MatrixMakeRotationY(fTheta);
	matRotZ = MatrixMakeRotationZ(fTheta);

	matWorld = MatrixMultiplyMatrix(matRotZ, matRotX); // Multiply the world matrix by the Z and X rotation matrices, so the object rotates around the Z and X axes
	matWorld = MatrixMultiplyMatrix(matWorld, matTrans); // Multiply the world matrix by the translation matrix, so the object is offset by the vector inputted to the translation matrix
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
		triangle triProjected, triTransformed; // Variables to represent the projected and transformed triangles of the object

		// Multiply the world matrix by each point of the triangle to get the transformed points
		triTransformed.p[0] = MatrixMultiplyVector(matWorld, tri.p[0]); 
		triTransformed.p[1] = MatrixMultiplyVector(matWorld, tri.p[1]);
		triTransformed.p[2] = MatrixMultiplyVector(matWorld, tri.p[2]);

		// Calculate the normal of the triangle
		vec3d normal, line1, line2;
		
		// Get the two lines of the triangle that extend from the first point of the triangle
		line1 = VectorSubtract(triTransformed.p[1], triTransformed.p[0]);
		line2 = VectorSubtract(triTransformed.p[2], triTransformed.p[0]);

		//The normal of the triangle is the cross prodict of these two lines
		normal = CrossProduct(line1, line2);
		
		//The normal must be normalized
		normal = Normalize(normal);

		//Make a ray from the triangle to camera
		vec3d vCameraRay = VectorSubtract(triTransformed.p[0], vCamera);

		//If the ray is aligned with the triangle's normal, then the triangle is visible to the camera, therefore draw it. Otherwise, don't draw it.
		if (DotProduct(normal, vCameraRay) < 0.0f)
		{
			// Project triangles from 3D to 2D
			triProjected.p[0] = MatrixMultiplyVector(matProj, triTransformed.p[0]);
			triProjected.p[1] = MatrixMultiplyVector(matProj, triTransformed.p[1]);
			triProjected.p[2] = MatrixMultiplyVector(matProj, triTransformed.p[2]);

			// Scale into view, then normalize the projected triangle to the screen
			//We divide by the w component of the projected triangle to scale it into view
			//The difference between the z component and the w component is that the z component is the distance from the camera to the object, while the w component is the distance from the camera to the object divided by the field of view. This is used to scale the object into view.
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