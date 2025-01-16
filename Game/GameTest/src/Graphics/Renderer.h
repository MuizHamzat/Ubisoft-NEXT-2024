#pragma once

#include "stdafx.h"
#include "mesh.h"
#include "src/Math/matrix.h"
#include "src/Math/vec3d.h"

class Renderer
{
private:
	struct Camera
	{
		float fYaw = 0; // Camera yaw
		
		vec3d pos; // Camera position
		vec3d lookDir; // Camera look direction
		vec3d target; // Camera target
		vec3d up; // Camera up direction

		mat4x4 cameraMatrix; // Camera matrix. This is used to convert world space into camera space
	};
	
	Camera camera; // Camera object

	mat4x4 matProj; // Projection matrix
	mat4x4 matView; // View matrix

	//Transform matrices & info
	float fTheta = 0.0f; // Rotation angle

	mat4x4 matScale = MatrixMakeScaling(1.0f, 1.0f, 1.0f); // Scaling matrix
	vec3d rotation = vec3d{ 0, 0, 0 }; // Rotation angles
    mat4x4 matRot = MatrixMakeRotation(rotation); // Rotation matrix around an arbitrary axis
	mat4x4 matTrans = MatrixMakeTranslation(0.0f, 0.0f, 0.0f); // Translation matrix

	mat4x4 matWorld = MatrixMakeIdentity(); // World matrix, which is the matrix that represents the position of the object in the world, and holds all the transformation information of the object

public:
	Renderer();

	void Init();

	void Update(float deltaTime);

	void Render(std::vector<Mesh>& meshes);

	//void SetCameraPos(vec3d cameraPos);

	void MoveCamera(float deltaTime);
};
