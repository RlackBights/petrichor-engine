#pragma once

#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/AtlasPage.h"
#include <string>
#include <map>

namespace PetrichorRendererAPI::Text {
    class Font
    {
    public:
        std::string name;
        int fontSize;

        std::map<char32_t, Character> characters;
        std::vector<AtlasPage> fontAtlases;

        Character GetCharacter(char32_t character);
    };
}