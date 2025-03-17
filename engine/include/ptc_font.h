#ifndef PTC_FONT_H
#define PTC_FONT_H

#define FONT_PATH "resources/fonts/"

#include <ptc_component.h>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <map>
#include <freetype2/freetype/freetype.h>

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