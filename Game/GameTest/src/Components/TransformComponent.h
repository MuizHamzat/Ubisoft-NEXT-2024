#pragma once

#include "stdafx.h"

#include "src/ECS/Component.h"

struct TransformComponent
{
	//The world matrix for the entity is calculated in a system using these variables
	vec3d position; //Position of the entity
	vec3d rotation = { 0, 0, 0 }; //Rotation of the entity
	vec3d scale = { 1, 1, 1 }; //Scale of the entity
	vec3d offset = { 0, 0, 0 }; //Offset of the entity

	mat4x4 matWorld; //World matrix for the entity
	bool isDirty = true; //Flag to check if the world matrix needs to be recalculated

	void UpdateWorldMatrix()
	{
		if (isDirty)
		{
			//Make transform matrices
			mat4x4 matScale = MatrixMakeScaling(scale.x, scale.y, scale.z);
			mat4x4 matRot = MatrixMakeRotation(rotation);
			mat4x4 matOffset = MatrixMakeTranslation(offset.x, offset.y, offset.z);
			mat4x4 matTrans = MatrixMakeTranslation(position.x, position.y, position.z);

			//Apply transforms to world matrix (scaling first, then rotations, then translations)
			matWorld = MatrixMultiplyMatrix(matScale, matRot);
			matWorld = MatrixMultiplyMatrix(matWorld, matTrans);	

			isDirty = false;
		}
	}
};
