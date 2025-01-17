#pragma once

#include "stdafx.h"

#include "src/ECS/Component.h"
#include "src/Graphics/mesh.h"
#include "src/Graphics/Color.h"

struct MeshComponent
{
	Mesh mesh;
	Color color;
};