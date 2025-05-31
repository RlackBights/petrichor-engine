#ifndef PTC_MATERIAL_HPP
#define PTC_MATERIAL_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ptc_shader.hpp"
#include "ptc_component.hpp"
#include "ptc_texture.hpp"

class Material : public Component
{
public:
	glm::vec4 baseColor;
	GLuint texture;
	GLuint specularMap;
	GLuint normalMap;
	Shader shader;
	float specularStrength;
	int specularExponent;

	Material(GLuint _texture = Texture::loadTexture("house.png"), GLuint _specularMap = -1, GLuint _normalMap = -1, glm::vec4 _baseColor = glm::vec4(1.0f), Shader _shader = Shader("vertex_shader.glsl", "fragment_shader.glsl"), float _specularStrength = 0.5f, int _specularExponent = 5);
};

#endif