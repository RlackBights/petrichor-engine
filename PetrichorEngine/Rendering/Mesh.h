#pragma once

#include "Rendering/Vertex.h"
#include <string>
#include <vector>

namespace PetrichorEngine::Rendering {
	struct Mesh
	{
	public:
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		int smoothing;
	};
}
