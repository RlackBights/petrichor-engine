#pragma once

#include "ECS/Component.h"
#include "Math/Math.h"
#include "Rendering/IShader.h"
#include "Rendering/Texture.h"

namespace PetrichorEngine::Rendering {
	using MaterialReference = uint32_t;

	struct Material : public ECS::Component
	{
	public:
		Math::Vector4 baseColor = {1, 1, 1, 1};

		TextureReference diffuse;
		TextureReference specular;
		TextureReference normal;

		ShaderReference shader;
		
		float specularStrength;
		int specularExponent;
	};
}
