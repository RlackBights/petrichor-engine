#ifndef PTC_FONT_HPP
#define PTC_FONT_HPP

#include "glm/fwd.hpp"
#include <string>
#define FONT_PATH "resources/fonts/"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>
#include <freetype/freetype.h> 

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class Font
{
private:
    static FT_Library ft;
    FT_Face face;
    static std::map<std::string, Font*> fontCache;
    std::string path;
    Font(std::string _path, int _fontSize);
public:
int fontSize;
    std::map<char, Character> characters;
    static Font* LoadFont(std::string _path, int _fontSize);
};

#endif