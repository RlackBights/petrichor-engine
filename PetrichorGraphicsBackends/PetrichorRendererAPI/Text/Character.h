#pragma once

namespace PetrichorRendererAPI::Text {
    struct Character {
        float UV[4] = {0, 0, 0, 0};        // Position of glyph in the atlas

        int  Size[2] = {0, 0};       // Size of glyph
        int  Bearing[2] = {0, 0};    // Offset from baseline to left/top of glyph
        unsigned int    Advance = 0;    // Offset to advance to next glyph

        int atlasPageIndex;
    };
}
