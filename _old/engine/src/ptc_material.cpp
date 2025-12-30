#include "ptc_texture.hpp"
#include <ptc_material.hpp>

Material::Material(GLuint _texture, GLuint _specularMap, GLuint _normalMap, glm::vec4 _baseColor, Shader _shader, float _specularStrength, int _specularExponent)
{
	shader = _shader;
	texture = _texture;
	specularMap = (_specularMap == -1) ? texture : _specularMap;
	normalMap = (_normalMap == -1) ? texture : _normalMap;
	baseColor = _baseColor;
	specularStrength = _specularStrength;
	specularExponent = _specularExponent;
}