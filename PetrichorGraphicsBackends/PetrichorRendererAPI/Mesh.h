#pragma once

#include "Vertex.h"
#include <ostream>
#include <string>
#include <vector>

namespace PetrichorRendererAPI {
	struct Mesh
	{
	public:
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		int smoothing;

		friend std::ostream& operator<<(std::ostream& stream, const Mesh& value)
		{
			stream << "{ Mesh: " << value.name << " }";
			return stream;
		}
	};
}
