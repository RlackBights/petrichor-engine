#pragma once

namespace PetrichorRendererAPI::Data {
	enum class LightType
	{
		Point,
		Directional,
		Spot
	};

	struct PointLight
	{
		float pos[3];
		float col[4];
	};

	struct DirectionalLight
	{
		float dir[3];
		float col[4];
	};

	struct SpotLight
	{
		float pos[3];
		float dir[3];
		float col[4];
	};

	
}
