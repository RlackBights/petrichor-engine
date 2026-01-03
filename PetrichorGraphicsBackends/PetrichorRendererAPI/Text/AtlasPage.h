#include "PetrichorRendererAPI/Texture.h"
#include <memory>

namespace PetrichorRendererAPI {
    struct AtlasPage
    {
        std::unique_ptr<Texture> atlasTexture;
        int penX = 0, penY = 0, rowHeight = 0;
    };
}