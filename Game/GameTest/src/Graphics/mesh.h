#pragma once

#include "stdafx.h"
#include "src/math/vec3d.h"
#include "Color.h"

struct triangle // 3 points of a triangle
{
	vec3d p[3];
	Color color;
};

int TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2);

// 3D mesh of an object made up of triangles
class Mesh {
public:
	std::vector<triangle> tris; // List of triangles

	// Load a mesh from an object file
	bool LoadFromObjectFile(std::string sFilename);
};
