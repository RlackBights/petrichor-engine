#pragma once

#include "PetrichorRendererAPI/Text/Font.h"
#include <string>

namespace PetrichorRendererAPI::Text
{
    class FontLoader
    {
    public:
        static Font LoadFont(const std::string& path, int fontSize, int atlasSize);
    };
}