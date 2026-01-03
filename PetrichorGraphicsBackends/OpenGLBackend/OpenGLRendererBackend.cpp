#include "OpenGLBackend/OpenGLRendererBackend.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "freetype/ftimage.h"
#include <algorithm>
#include <filesystem>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace RendererBackends {
    Text::Font OpenGLRendererBackend::LoadFont(const std::string& path, int fontSize, int atlasSize)
    {
        Text::Font font;

        font.name = std::filesystem::path(path).stem();
        font.fontAtlases.push_back({});

        FT_Library  ft;
        FT_Face     face;

        FT_Init_FreeType(&ft);
        FT_New_Face(ft, path.c_str(), 0, &face);
        FT_Set_Char_Size(face, 0, fontSize << 6, 96, 96);

        fontCache.insert({&font, face});
        return font;
    }

    Text::Character OpenGLRendererBackend::LoadGlyph(char32_t character, Text::Font* font)
    {
        auto characterIterator = font->characters.find(character);
        if (characterIterator != font->characters.end())
            return characterIterator->second;
        
        FT_Face face = fontCache[font];
        FT_Load_Char(face, character, FT_LOAD_RENDER);

        return ProcessGlyph(character, font, face->glyph);
    }

    Text::Character ProcessGlyph(char32_t character, Text::Font* font, FT_GlyphSlot glyph)
    {
        FT_Bitmap& bmp = glyph->bitmap;
        AtlasPage& page = font->fontAtlases.back();

        if (page.penX + bmp.width >= page.atlasTexture->width) {
            page.penX = 0;
            page.penY += page.rowHeight + 1;
            page.rowHeight = 0;
        }

        if (page.penY + bmp.rows >= page.atlasTexture->height) {
            font->fontAtlases.push_back({nullptr, 0, 0, font->fontSize});
            return ProcessGlyph(character, font, glyph);
        }

        for (int y = 0; y < bmp.rows; ++y)
            memcpy(&page.atlasTexture->pixels[(page.penY + y) * page.atlasTexture->width + page.penX],
                &bmp.buffer[y * bmp.pitch],
                bmp.width);

        Text::Character c;
        c.Size[0]    = bmp.width;
        c.Size[1]    = bmp.rows;
        c.Bearing[0] = glyph->bitmap_left;
        c.Bearing[1] = glyph->bitmap_top;
        c.Advance    = glyph->advance.x >> 6;
        c.atlasPageIndex  = font->fontAtlases.size() - 1;

        c.UV[0] = float(page.penX) / page.atlasTexture->width;
        c.UV[1] = float(page.penY) / page.atlasTexture->height;
        c.UV[2] = float(page.penX + bmp.width) / page.atlasTexture->width;
        c.UV[3] = float(page.penY + bmp.rows) / page.atlasTexture->height;

        font->characters.insert({character, c});

        page.rowHeight = std::max(page.rowHeight, int(bmp.rows));
        page.penX += bmp.width + 1;

        return font->characters[character];
    }
}