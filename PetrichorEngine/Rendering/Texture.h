#pragma once

#include <cstdint>

namespace PetrichorEngine::Rendering
{
	using TextureReference = uint32_t;

	class Texture
	{
	public:
		TextureReference handle;
		int width;
		int height;

		Texture(uint32_t handle = 0, int width = 0, int height = 0);
	};
}