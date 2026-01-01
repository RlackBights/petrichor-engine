#pragma once

#include "Math/Math.h"
#include "Rendering/IShader.h"
#include "Rendering/Texture.h"
#include <memory>

namespace PetrichorEngine::Rendering {

	struct Material
	{
	public:
		Math::Vector4 baseColor = {1, 1, 1, 1};

		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;
		std::shared_ptr<Texture> normal;

		std::shared_ptr<IShader> shader;
		
		float specularStrength;
		int specularExponent;
	};
}
