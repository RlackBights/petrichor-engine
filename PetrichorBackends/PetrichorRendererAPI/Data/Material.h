#pragma once

#include "IShaderProgram.h"
#include "Texture.h"
#include <array>
#include <memory>

namespace PetrichorRendererAPI::Data {

	struct Material
	{
	public:
		std::array<float, 4> baseColor = {1, 1, 1, 1};

		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;
		std::shared_ptr<Texture> normal;

		std::shared_ptr<IShaderProgram> shader;
		
		float specularStrength;
		int specularExponent;

		Material(const std::array<float, 4>& baseColor = {4, 4, 4, 4}, const std::shared_ptr<IShaderProgram> shader = nullptr, const std::shared_ptr<Texture>& diffuse = nullptr, const std::shared_ptr<Texture>& specular = nullptr, float specularStrength = 1.0f, int specularExponent = 4, const std::shared_ptr<Texture>& normal = nullptr)
		{
			this->baseColor = baseColor;
			this->shader = shader;
			this->diffuse = diffuse;
			this->specular = specular;
			this->normal = normal;
			this->specularStrength = specularStrength;
			this->specularExponent = specularExponent;
		}
	};
}
