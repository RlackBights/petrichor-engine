#include "PetrichorRendererAPI/Text/FontLoader.h"
#include "PetrichorRendererAPI/Renderer.h"
#include "PetrichorRendererAPI/Text/Font.h"

namespace PetrichorRendererAPI::Text {
    Font FontLoader::LoadFont(const std::string &path, int fontSize, int atlasSize) { return Renderer::Get()->LoadFont(path, fontSize, atlasSize); }
}