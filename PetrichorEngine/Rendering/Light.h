#pragma once

#include "ECS/Component.h"
#include "Math/Math.h"
namespace PetrichorEngine::Rendering {
	enum LightType
	{
		Point,
		Directional,
		Spot
	};

	struct PointLight
	{
		Math::Vector3 pos;
		Math::Vector4 col;
	};

	typedef struct DirectionalLight
	{
		Math::Vector3 dir;
		Math::Vector4 col;
	} DirectionalLight;

	typedef struct SpotLight
	{
		Math::Vector3 pos;
		Math::Vector3 dir;
		Math::Vector4 col;
	} SpotLight;

	class Light : public ECS::Component
	{
	friend class Reflection;
	private:
		static std::vector<Light*> lights;
	public:
		static glm::vec3 ambientLight;
		static float ambientLightIntensity;
		glm::vec3 lightColor;
		LightType type;
		float cutoff;
		float focus;

		Light(LightType _type = LightType::Point, glm::vec3 _lightColor = glm::vec3(1.0f), float _cutoff = 0.2f, float _focus = 50.0f);

		static glm::vec3 EulerToDirection(const glm::vec3& eulerAngles);
		static glm::vec3 QuaternionToDirection(const glm::quat& quaternion);
		static std::vector<Light*> GetLights(LightType _type);
		static std::vector<glm::vec3> GetLightDirections(LightType _type);
		static std::vector<glm::vec3> GetLightPositions(LightType _type);
		static std::vector<glm::vec3> GetLightColors(LightType _type);
		static std::vector<float> GetSpotlightFocus();
		static std::vector<float> GetSpotlightCutoff();
		static std::vector<PointLight> GetPointLights();
	};
}
