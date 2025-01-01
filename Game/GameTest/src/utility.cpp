#include "stdafx.h"
#include "utility.h"
#include "App/app.h"
#include <sstream> // For std::ostringstream


namespace Utility
{
	std::string VectorToString(const std::vector<float>& vec)
	{
		std::ostringstream oss; // Create an output string stream
		for (size_t i = 0; i < vec.size(); i++) // Iterate over the elements of the vector
		{
			oss << vec[i]; // Append the current element to the string
			// If this is not the last element, append a comma
			if (i != vec.size() - 1)
			{
				oss << ", ";
			}
		}
		return oss.str();
	}

	void DrawShape(std::vector<std::vector<float>>& points, float r, float g, float b) // Given a vector of points, draw lines between them
	{
		for (size_t i = 0; i < points.size(); i++) // Iterate over the points
		{
			std::vector<float> &start = points[i]; // Get the current point
			std::vector<float> &end = points[(i + 1) % points.size()]; // Get the next point, wrapping around to the first point if necessary
			App::DrawLine(start[0], start[1], end[0], end[1],r,g,b); // Draw a line between the two points
		}
	}
}