#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace PetrichorRendererAPI
{
	struct Texture
	{
	private:
		void* backendHandle = nullptr;
	public:
		int width;
		int height;
		int channels;
		std::vector<uint8_t> pixels;

		bool isSRGB;
		bool generateMipMaps;
		std::string source;
	};
}