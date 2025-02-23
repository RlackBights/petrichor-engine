#include <ptc_material.h>

Material::Material()
{
	baseColor = glm::vec4(1.0f);
	texture = Texture::loadTexture("default_texture.jpg");
	specularMap = texture;
	normalMap = texture;
	shader = Shader("vertex_shader.glsl", "fragment_shader.glsl");
	specularStrength = 0.5f;
	specularExponent = 5;
}
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