#ifndef PTC_TEXT_H
#define PTC_TEXT_H

#include <functional>
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
	std::string text;
	glm::vec2 position;
	glm::vec2 offset;
    unsigned int VAO, VBO;
	Font* font;
	glm::vec4 color;
	std::function<float(float _x)> animationFunction;

	void FixedUpdate() override;
public:
    Shader textShader;

	int getPixelWidth(int _index = 0, bool _ignoreLinebreak = false);
    Text(std::string _text, float _x = 0.0f, float _y = 0.0f, Font* _font = Font::LoadFont("arial.ttf", 48), glm::vec4 _color = glm::vec4(1.0f), Shader _shader = Shader("text_vert.glsl", "text_frag.glsl"));
	void SetTextAnimation(std::function<float(float _x)> _animationFunction);
	void MoveText(int _x, int _y, bool _centered = true);
	void CenterText();
	void SetTextColor(glm::vec4 _color);
	glm::vec4 GetTextColor();
	void SetText(std::string _text, bool _updatePosition = true);
	std::string GetText();
	void Awake() override;
	Font* GetFont();
};

#endif