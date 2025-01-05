#include "stdafx.h"
#include "mesh.h"

// Clip a triangle against a plane
int TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2)
{
	// Make sure the plane normal is normalized
	plane_n = Normalize(plane_n);
	// Create two temporary lists of vertices
	std::vector<vec3d> insidePoints;
	std::vector<vec3d> outsidePoints;
	// Calculate the distance of each point in the triangle to the plane
	auto dist = [&](vec3d& p)
		{
			vec3d n = Normalize(p);
			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - DotProduct(plane_n, plane_p));
		};
	// Get the distances of each point in the triangle to the plane
	float d0 = dist(in_tri.p[0]);
	float d1 = dist(in_tri.p[1]);
	float d2 = dist(in_tri.p[2]);
	// If the distance of the first point is greater than 0, it is inside the plane
	if (d0 >= 0) insidePoints.push_back(in_tri.p[0]);
	else outsidePoints.push_back(in_tri.p[0]);
	// If the distance of the second point is greater than 0, it is inside the plane
	if (d1 >= 0) insidePoints.push_back(in_tri.p[1]);
	else outsidePoints.push_back(in_tri.p[1]);
	// If the distance of the third point is greater than 0, it is inside the plane
	if (d2 >= 0) insidePoints.push_back(in_tri.p[2]);
	else outsidePoints.push_back(in_tri.p[2]);
	// If all points are inside the plane, return 1
	if (insidePoints.size() == 3)
	{
		out_tri1 = in_tri;
		return 1;
	}
	// If all points are outside the plane, return 0
	if (insidePoints.size() == 0)
	{
		return 0;
	}
	// If two points are inside the plane, clip the triangle
	if (insidePoints.size() == 1 && outsidePoints.size() == 2)
	{
		// Copy the inside point to the first point of the output triangle
		out_tri1.p[0] = insidePoints[0];
		// Calculate the intersection point of the two outside points with the plane
		out_tri1.p[1] = VectorIntersectPlane(plane_p, plane_n, insidePoints[0], outsidePoints[0]);
		out_tri1.p[2] = VectorIntersectPlane(plane_p, plane_n, insidePoints[0], outsidePoints[1]);
		return 1; // Return 1 triangle
	}
	// If one point is inside the plane, clip the triangle
	if (insidePoints.size() == 2 && outsidePoints.size() == 1)
	{
		// Copy the inside points to the first two points of the output triangle
		out_tri1.p[0] = insidePoints[0];
		out_tri1.p[1] = insidePoints[1];
		// Calculate the intersection point of the inside points with the plane
		out_tri1.p[2] = VectorIntersectPlane(plane_p, plane_n, insidePoints[0], outsidePoints[0]);
		
		// Copy the inside points to the first two points of the second output triangle
		out_tri2.p[0] = insidePoints[1];
		out_tri2.p[1] = out_tri1.p[2];
		// Calculate the intersection point of the inside points with the plane
		out_tri2.p[2] = VectorIntersectPlane(plane_p, plane_n, insidePoints[1], outsidePoints[0]);

		return 2; // Return 2 triangles
	}
}


bool Mesh::LoadFromObjectFile(std::string sFilename)
{
	//Open file
	std::ifstream f(sFilename);
	if (!f.is_open())
		return false;

	//Local cache of verts
	std::vector<vec3d> verts;

	while (!f.eof()) // While we are not at the end of the file
	{
		//Get the line
		char line[128];
		f.getline(line, 128); // the two parameters are the buffer (where the line will be stored) and the size of the buffer

		//Put the line into a string stream
		// A stringstream is a stream (like cout or cin) that can be used to read from or write to strings
		std::stringstream s;
		s << line;

		char junk;

		//Check what type of line it is
		if (line[0] == 'v' && line[1] != 't' && line[1] != 'n')
		{
			vec3d v;
			s >> junk >> v.x >> v.y >> v.z; // Read the line into a vec3d
			verts.push_back(v); // Add the vertex to the mesh's list of vertices
		}

		if (line[0] == 'f')
		{
			std::string vertexData[3];
			s >> junk >> vertexData[0] >> vertexData[1] >> vertexData[2]; // Read the groups of vertex data into strings

			int f[3];
			for (int i = 0; i < 3; i++)
			{
				if (vertexData[i].find('/') != std::string::npos)
				{
					// Format with '/' (e.g., 45/78/32)
					std::stringstream vertexStream(vertexData[i]);
					std::string firstInt;
					std::getline(vertexStream, firstInt, '/'); // Get the substring before the first '/'
					f[i] = std::stoi(firstInt); // Convert to integer
				}
				else
				{
					// Simple format without '/' (e.g., 22)
					f[i] = std::stoi(vertexData[i]); // Convert directly to integer
				}
			}

			// Add the triangle to the mesh's list of triangles
			tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
		}

	}

	return true;
}