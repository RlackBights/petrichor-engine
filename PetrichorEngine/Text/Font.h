#pragma once

#include "Math/Math.h"
#include "Rendering/Texture.h"
#include <memory>
#include <string>
#include <map>

namespace PetrichorEngine::Text {
    struct Character {
        std::shared_ptr<Rendering::Texture>     TextureID;  // Reference to the character texture
        Math::Vector2i                  Size;       // Size of glyph
        Math::Vector2i                  Bearing;    // Offset from baseline to left/top of glyph
        unsigned int                    Advance;    // Offset to advance to next glyph
    };

    class Font
    {
    public:
        int fontSize;
        std::map<char, Character> characters;
        std::string name;

        Font(std::string name, int fontSize);
    };
}