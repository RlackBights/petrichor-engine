#ifndef PTC_TEXT_H
#define PTC_TEXT_H

#include "ptc_component.h"
#include "ptc_font.h"
#include <ft2build.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <ptc_shader.h>
#include <map>
#include FT_FREETYPE_H

class Text : public Component
{
public:
	static std::map<char, Character> Characters;
	static FT_Library ft;
	static FT_Face face;
    static unsigned int VAO, VBO;
    static Shader textShader;
	Font* font;

    static void initTextManager(Shader inputShader, int screenWidth, int screenHeight);
	static void renderText(std::string text, float x = 0.0f, float y = 0.0f, float scale = 1.0f, int screenWidth = 800, int screenHeight = 600, glm::vec3 color = glm::vec3(1.0f), bool isUI = true);
};

#endif