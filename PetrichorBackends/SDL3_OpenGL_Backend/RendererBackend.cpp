#include "SDL3_OpenGL_Backend/RendererBackend.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Triangle.h"
#include "freetype/ftimage.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>
#include <algorithm>
#include <filesystem>
#include <ft2build.h>
#include <iostream>
#include FT_FREETYPE_H

namespace RendererBackends::SDL3_OpenGL {
    Text::Font RendererBackend::LoadFont(const std::string& path, int fontSize, int atlasSize)
    {
        Text::Font font;

        font.name = std::filesystem::path(path).stem().string();
        font.fontAtlases.push_back({});

        FT_Library  ft;
        FT_Face     face;

        FT_Init_FreeType(&ft);
        FT_New_Face(ft, path.c_str(), 0, &face);
        FT_Set_Char_Size(face, 0, fontSize << 6, 96, 96);

        fontCache.insert({&font, face});
        return font;
    }

    Text::Character RendererBackend::LoadGlyph(char32_t character, Text::Font* font)
    {
        auto characterIterator = font->characters.find(character);
        if (characterIterator != font->characters.end())
            return characterIterator->second;
        
        FT_Face face = fontCache[font];
        FT_Load_Char(face, character, FT_LOAD_RENDER);

        return ProcessGlyph(character, font, face->glyph);
    }

    Text::Character RendererBackend::ProcessGlyph(char32_t character, Text::Font* font, FT_GlyphSlot glyph)
    {
        FT_Bitmap& bmp = glyph->bitmap;
        Text::AtlasPage& page = font->fontAtlases.back();

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

    void RendererBackend::SetWindowFlags(SDL_WindowFlags flags) { this->flags = flags; }

    void RendererBackend::InitializeWindow(const std::string& title, int width, int height)
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
        {
            std::cout << "[ERROR] SDL3 couldn't be initialized! " << SDL_GetError();
            return;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        
        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

        window = SDL_CreateWindow(title.c_str(), int(width * main_scale), int(height * main_scale), (flags == 0 ? SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY : flags));
        if (!window)
        {
            std::cout << "[ERROR] SDL3 window couldn't be created! " << SDL_GetError() << "\r\n";
            return;
        }

        glContext = SDL_GL_CreateContext(window);
        if (!glContext)
        {
            std::cout << "[ERROR] SDL3 OpenGL context couldn't be created! " << SDL_GetError() << "\r\n";
            return;
        }

        SDL_GL_MakeCurrent(window, glContext);
        SDL_GL_SetSwapInterval(1); // VSync
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_StartTextInput(window);

        SDL_ShowWindow(window);
    }

    void RendererBackend::InitializeRenderer()
    {
        std::cout << "[ERROR] Renderer initialization unimplemented!\r\n";
    }

    void RendererBackend::InitializeFrame()
    {
        std::cout << "[ERROR] Frame initialization unimplemented!\r\n";
    }

    void RendererBackend::DrawTriangle(const Triangle& triangle, const Material& material)
    {
        std::cout << "[ERROR] Triangle drawing unimplemented!\r\n";
    }

    void RendererBackend::DrawQuad(const Quad& quad, const Material& material)
    {
        std::cout << "[ERROR] Quad drawing unimplemented!\r\n";
    }

    void RendererBackend::DrawMesh(const Mesh& mesh, const Material& material)
    {
        std::cout << "[ERROR] Mesh drawing unimplemented!\r\n";
    }

    void RendererBackend::WrapFrame()
    {
        std::cout << "[ERROR] Frame wrapping unimplemented!\r\n";
    }
}