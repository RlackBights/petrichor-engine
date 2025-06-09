#ifndef PTC_TEXT_HPP
#define PTC_TEXT_HPP

#include <functional>
#include "ptc_component.hpp"
#include "ptc_font.hpp"
#include <ft2build.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "ptc_gui.hpp"
#include "ptc_gui_structs.hpp"
#include "ptc_shader.hpp"
#include <freetype/freetype.h>

class Text : public Component
{
friend class GUI;
private:
	std::string text;
	glm::vec2 position;
	glm::vec2 offset;
    unsigned int VAO, VBO;
	Font* font;
	glm::vec4 color;
	std::function<float(float _x)> animationFunction;
	int getPixelWidthSelf(int _index = 0, bool _ignoreLinebreak = false);
	static Text staticText;

	void FixedUpdate() override;
public:
    Shader textShader;

	int getPixelWidth(const std::string& text, int _index = 0, bool _ignoreLinebreak = false);
	Text();
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
	void ForceDrawText(const glm::vec2& position, const Rect& scissor);
	static void StaticDrawText(const std::string& text, const glm::vec2& position, const Rect& scissor);
	static int getStaticPixelWidth(const std::string& text, const int fontSize);
};

#endif