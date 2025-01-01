#pragma once

#include "stdafx.h"
#include "src/math/vec3d.h"

struct triangle // 3 points of a triangle
{
	vec3d p[3];
};

// 3D mesh of an object made up of triangles
class Mesh {
public:
	std::vector<triangle> tris; // List of triangles

	// Load a mesh from an object file
	bool LoadFromObjectFile(std::string sFilename);
};
