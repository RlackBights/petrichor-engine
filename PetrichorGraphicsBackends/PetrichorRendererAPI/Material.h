#pragma once

#include "IShader.h"
#include "Texture.h"
#include <memory>

namespace PetrichorRendererAPI {

	struct Material
	{
	public:
		float baseColor[4] = {1, 1, 1, 1};

		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;
		std::shared_ptr<Texture> normal;

		std::shared_ptr<IShader> shader;
		
		float specularStrength;
		int specularExponent;
	};
}
