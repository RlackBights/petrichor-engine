#pragma once

#include "ECS/Component.h"
#include "Math/Math.h"
#include "PetrichorRendererAPI/LightData.h"
#include <vector>

namespace PetrichorEngine::Rendering {
    class Light : public ECS::Component
	{
	friend class Reflection;
	private:
		static std::vector<Light*> lights;
	public:
		static Math::Vector4 ambientLight;
		static float ambientLightIntensity;
		Math::Vector4 lightColor;
		PetrichorRendererAPI::LightType type;
		float cutoff;
		float focus;

		Light(PetrichorRendererAPI::LightType _type = PetrichorRendererAPI::LightType::Point, Math::Vector4 _lightColor = Math::Vector4(1.0f), float _cutoff = 0.2f, float _focus = 50.0f);

		static glm::vec3 EulerToDirection(const glm::vec3& eulerAngles);
		static glm::vec3 QuaternionToDirection(const glm::quat& quaternion);
		static std::vector<Light*> GetLights(PetrichorRendererAPI::LightType _type);
		static std::vector<glm::vec3> GetLightDirections(PetrichorRendererAPI::LightType _type);
		static std::vector<glm::vec3> GetLightPositions(PetrichorRendererAPI::LightType _type);
		static std::vector<glm::vec3> GetLightColors(PetrichorRendererAPI::LightType _type);
		static std::vector<float> GetSpotlightFocus();
		static std::vector<float> GetSpotlightCutoff();
		static std::vector<PetrichorRendererAPI::PointLight> GetPointLights();
	};
}