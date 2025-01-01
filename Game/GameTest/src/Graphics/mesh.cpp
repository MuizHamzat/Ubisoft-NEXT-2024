#include "stdafx.h"
#include "mesh.h"

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
			int f[3];
			char slash; // To skip the '/' characters
			int temp;   // To discard the values after the first integers in each group

			// Parse the line for the face indices
			s >> junk; // Skip the 'f' character
			for (int i = 0; i < 3; i++) {
				s >> f[i]; // Read the first integer (vertex index)
				if (s.peek() == '/') {
					s >> slash >> temp; // Skip over the '/' and discard the second value
					if (s.peek() == '/') {
						s >> slash >> temp; // Skip over the second '/' and discard the third value
					}
				}
			}

			// Use the first integers as vertex indices
			tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
		}

	}

	return true;
}