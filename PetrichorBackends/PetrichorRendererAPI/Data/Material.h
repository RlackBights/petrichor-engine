#pragma once

#include "IShaderProgram.h"
#include "Texture.h"
#include <memory>

namespace PetrichorRendererAPI::Data {

	struct Material
	{
	public:
		float baseColor[4] = {1, 1, 1, 1};

		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;
		std::shared_ptr<Texture> normal;

		std::shared_ptr<IShaderProgram> shader;
		
		float specularStrength;
		int specularExponent;
	};
}
