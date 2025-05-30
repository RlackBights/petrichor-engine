#ifndef PTC_LIGHT_H
#define PTC_LIGHT_H

#include <glm/glm.hpp>
#include <vector>
#include "ptc_component.h"

typedef enum LightType
{
	Point,
	Directional,
	Spot
} LightType;

typedef struct PointLight
{
	glm::vec4 pos;
	glm::vec4 col;
} PointLight;

typedef struct DirectionalLight
{
	glm::vec4 dir;
	glm::vec4 col;
} DirectionalLight;

typedef struct SpotLight
{
	glm::vec4 pos;
	glm::vec4 dir;
	glm::vec4 col;
} SpotLight;

class Light : public Component
{
private:
	static std::vector<Light*> lights;
public:
	static glm::vec3 ambientLight;
	static float ambientLightIntensity;
	glm::vec3 lightColor;
	LightType type;
	float cutoff;
	float focus;

	Light(LightType _type, glm::vec3 _lightColor = glm::vec3(1.0f), float _cutoff = 0.2f, float _focus = 50.0f);

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

#endif