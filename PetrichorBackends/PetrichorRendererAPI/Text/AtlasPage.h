#pragma once

#include "PetrichorRendererAPI/Data/Texture.h"
#include <memory>

namespace PetrichorRendererAPI::Text {
    struct AtlasPage
    {
        std::unique_ptr<Data::Texture> atlasTexture;
        int penX = 0, penY = 0, rowHeight = 0;
    };
}