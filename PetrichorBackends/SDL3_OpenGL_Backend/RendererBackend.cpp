#include <glad/glad.h>
#include "SDL3_OpenGL_Backend/RendererBackend.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/Data/Texture.h"
#include "PetrichorRendererAPI/Text/AtlasPage.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Triangle.h"
#include "freetype/ftimage.h"
#include "freetype/fttypes.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_opengl.h>
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <ft2build.h>
#include <iostream>
#include <memory>
#include <vector>
#include FT_FREETYPE_H

namespace RendererBackends::SDL3_OpenGL {
    Text::Font* RendererBackend::LoadFont(const std::string& path, int fontSize, int atlasSize)
    {
        auto font = std::make_shared<Text::Font>();

        font->name = std::filesystem::path(path).stem().string();
        font->fontSize = fontSize;
        font->atlasSize = atlasSize;
        font->fontAtlases = std::vector<Text::AtlasPage>();
        font->fontAtlases.push_back({std::make_unique<Texture>(), 0, 0, 0});
        font->fontAtlases[0].atlasTexture->width = atlasSize;
        font->fontAtlases[0].atlasTexture->height = atlasSize;
        font->fontAtlases[0].atlasTexture->pixels.resize(atlasSize * atlasSize);

        FT_Library      ft;
        FT_Face     face;

        FT_Error e;
        e = FT_Init_FreeType(&ft);
        if (e != FT_Err_Ok) { std::cout << FT_Error_String(e) << "\r\n"; }
        e = FT_New_Face(ft, path.c_str(), 0, &face);
        if (e != FT_Err_Ok) { std::cout << "FT_New_Face failed with error code: " << e << "\r\n"; }
        e = FT_Set_Char_Size(face, 0, fontSize << 6, 96, 96);
        if (e != FT_Err_Ok) { std::cout << FT_Error_String(e) << "\r\n"; }
        
        fontCache.insert({font, std::make_unique<FT_Face>(face)});
        return font.get();
    }

    Text::Character RendererBackend::LoadGlyph(char32_t character, Text::Font* font)
    {
        auto characterIterator = font->characters.find(character);
        if (characterIterator != font->characters.end())
            return characterIterator->second;
        
        FT_Face face;
        for (auto& [_font, _face] : fontCache) {
            if (font->name == _font->name && font->fontSize == _font->fontSize)
            {
                face = *_face.get();
                break;
            }
        }
        FT_Load_Char(face, character, FT_LOAD_RENDER);

        return ProcessGlyph(character, font, face->glyph);
    }

    Text::Character RendererBackend::ProcessGlyph(char32_t character, Text::Font* font, FT_GlyphSlot glyph)
    {
        FT_Bitmap& bmp = glyph->bitmap;
        Text::AtlasPage& page = font->fontAtlases.back();
        if (page.atlasTexture == nullptr)
        {
            page.atlasTexture = std::make_unique<Texture>();
            page.atlasTexture->width = font->atlasSize;
            page.atlasTexture->height = font->atlasSize;
            page.atlasTexture->pixels.resize(font->atlasSize * font->atlasSize);
        }

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
        {
            uint8_t* dst = &page.atlasTexture->pixels[(page.penY + y) * page.atlasTexture->width + page.penX];
            uint8_t* src = &bmp.buffer[y * bmp.pitch];

            memcpy(dst, src, bmp.width);
        }

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
    void RendererBackend::SetCursorIcon(uint32_t cursorID) { SDL_SetCursor(SDL_CreateSystemCursor(SDL_SystemCursor(cursorID))); }

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

        screen = Rect(0, 0, width, height);

        SDL_GL_MakeCurrent(window, glContext);
        SDL_GL_SetSwapInterval(1); // VSync
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_StartTextInput(window);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cout << "[ERROR] Failed to initialize GLAD!\n";
            return;
        }

        SDL_ShowWindow(window);
    }

    Rect RendererBackend::GetScreenRect()
    {
        return screen;
    }

    void RendererBackend::ResizeWindow(int width, int height)
    {
        screen = Rect(screen.x, screen.y, width, height);
    }

    void RendererBackend::InitializeRenderer()
    {
        std::cout << "[ERROR] Renderer initialization unimplemented!\r\n";
    }

    void RendererBackend::InitializeFrame()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererBackend::DrawText(const std::string text, Text::Font* font, const Data::Material& material)
    {
        std::cout << "[ERROR] Text drawing unimplemented!\r\n";
    }

    void RendererBackend::DrawRect(const Rect& rect, const Material& material)
    {
        std::cout << "[ERROR] Rect drawing unimplemented!\r\n";
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
        SDL_GL_SwapWindow(window);
    }
}