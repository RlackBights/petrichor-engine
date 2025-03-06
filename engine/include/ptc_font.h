#ifndef PTC_FONT_H
#define PTC_FONT_H

#define FONT_PATH "resources/fonts/"

#include <ptc_component.h>
#include <map>

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

class Font : public Component
{
private:
    static std::map<std::string, Font*> fontCache;
    std::string path;
    int fontSize;

public:
    Font(std::string _path, int _fontSize);
};

#endif