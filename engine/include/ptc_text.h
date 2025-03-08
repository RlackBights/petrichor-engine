#ifndef PTC_TEXT_H
#define PTC_TEXT_H

#include <ptc_component.h>
#include <ptc_renderer.h>
#include <ptc_font.h>
#include <ft2build.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <ptc_shader.h>
#include <freetype2/freetype/freetype.h>

class Text : public Component
{
private:
	glm::vec2 position;
public:
    unsigned int VAO, VBO;
    Shader textShader;
	Font* font;
	glm::vec4 color;
	std::string text;

    Text(std::string text, float x = 0.0f, float y = 0.0f, Font* _font = Font::LoadFont("arial.ttf", 48), glm::vec4 color = glm::vec4(1.0f), Shader _shader = Shader("text_vert.glsl", "text_frag.glsl"));
	void FixedUpdate() override;
};

#endif