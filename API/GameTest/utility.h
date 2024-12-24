#pragma once

#include "stdafx.h"

namespace Utility
{
	// Convert a vector of floats to a string, separated by commas
	std::string VectorToString(const std::vector<float>& vec);
	void DrawShape(std::vector<std::vector<float>> &points, float r=1.0f, float g=1.0f, float b=1.0f);
}
