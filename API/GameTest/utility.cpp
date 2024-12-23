#include "stdafx.h"
#include "utility.h"

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
}