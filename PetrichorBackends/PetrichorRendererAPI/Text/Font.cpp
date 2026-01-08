#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Renderer.h"
#include "PetrichorRendererAPI/Text/Character.h"

namespace PetrichorRendererAPI::Text {

    Character Font::GetCharacter(char32_t character)
    {
        auto charIterator = characters.find(character);
        if (charIterator != characters.end())
            return charIterator->second;
        
        return Renderer::Get()->LoadGlyph(character, this);
    }
}