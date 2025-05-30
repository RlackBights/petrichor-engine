#include <ptc_light.hpp>

Light::Light(LightType _type, glm::vec3 _lightColor, float _cutoff, float _focus) : lightColor(_lightColor), type(_type), cutoff(_cutoff), focus(_focus) { lights.push_back(this); }

glm::vec3 Light::EulerToDirection(const glm::vec3& eulerAngles) {
	float pitchRad = glm::radians(eulerAngles.x);
	float yawRad = glm::radians(eulerAngles.y);
	float rollRad = glm::radians(eulerAngles.z);

	float x = cos(pitchRad) * sin(yawRad);
	float y = sin(pitchRad);
	float z = cos(pitchRad) * cos(yawRad);

	return glm::normalize(glm::vec3(x, y, z));
}
glm::vec3 Light::QuaternionToDirection(const glm::quat& quaternion) {
    return glm::normalize(quaternion * glm::vec3(0.0f, 0.0f, -1.0f));
}
std::vector<Light*> Light::GetLights(LightType _type)
{
	std::vector<Light*> enabledLights;
	for (Light* light : lights) if (light->enabled && light->parentObject->enabled && light->type == _type) enabledLights.push_back(light);
	return enabledLights;
}
std::vector<glm::vec3> Light::GetLightDirections(LightType _type)
{
	std::vector<glm::vec3> dir;
	for (Light* light : GetLights(_type)) dir.push_back(Light::QuaternionToDirection(light->parentObject->transform.rotation));
	return dir;
}
std::vector<glm::vec3> Light::GetLightPositions(LightType _type)
{
	std::vector<glm::vec3> pos;
	for (Light* light : GetLights(_type)) pos.push_back(light->parentObject->transform.position);
	return pos;
}
std::vector<glm::vec3> Light::GetLightColors(LightType _type)
{
	std::vector<glm::vec3> col;
	for (Light* light : GetLights(_type)) col.push_back(light->lightColor);
	return col;
}
std::vector<float> Light::GetSpotlightFocus()
{
	std::vector<float> focus;
	for (Light* light : GetLights(Spot)) focus.push_back(light->focus);
	return focus;
}
std::vector<float> Light::GetSpotlightCutoff()
{
	std::vector<float> cutoff;
	for (Light* light : GetLights(Spot)) cutoff.push_back(light->cutoff);
	return cutoff;

}
std::vector<PointLight> Light::GetPointLights()
{
	std::vector<PointLight> l;
	for (Light* light : GetLights(Point))
	{
		PointLight pl = PointLight();
		pl.pos = glm::vec4(light->parentObject->transform.position, 1.0f);
		pl.col = glm::vec4(light->lightColor, 1.0f);
		l.push_back(pl);
	}
	return l;
}

glm::vec3 Light::ambientLight;
float Light::ambientLightIntensity;
std::vector<Light*> Light::lights;