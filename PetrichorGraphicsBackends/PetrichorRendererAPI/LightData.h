#pragma once

namespace PetrichorRendererAPI {
	enum LightType
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

	typedef struct DirectionalLight
	{
		float dir[3];
		float col[4];
	} DirectionalLight;

	typedef struct SpotLight
	{
		float pos[3];
		float dir[3];
		float col[4];
	} SpotLight;

	
}
