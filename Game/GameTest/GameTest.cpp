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
	renderer.Init();

	meshCube.LoadFromObjectFile("TestData/axis.obj");
	meshes.push_back(meshCube);
}

void Update(float deltaTime)
{
	renderer.Update(deltaTime);
}

void Render()
{
	renderer.Render(meshes);
}

void Shutdown()
{
	// No resources to clean up for line-drawn objects
}