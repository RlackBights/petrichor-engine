#include <glad/glad.h>
#include "SDL3_OpenGL_Backend/RendererBackend.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/Data/Texture.h"
#include "PetrichorRendererAPI/Text/AtlasPage.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Triangle.h"
#include "SDL3_OpenGL_Backend/Shader.h"
#include "SDL3_OpenGL_Backend/ShaderProgram.h"
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
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include FT_FREETYPE_H

namespace RendererBackends::SDL3_OpenGL {

    void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
                                    GLsizei length, const GLchar* message, const void* userParam) {
        
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
            return; 
        }

        std::cerr << "[OpenGL] " << message << "\r\n";

        if (type == GL_DEBUG_TYPE_ERROR) {
            #if defined(_MSC_VER)
                __debugbreak();
            #elif defined(__GNUC__) || defined(__clang__)
                __builtin_trap();
            #endif
        }
    }

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
        if (e != FT_Err_Ok) { std::cout << "FT_New_Face failed with error code: " << FT_Error_String(e) << " while loading font " << path << "\r\n"; }
        e = FT_Set_Pixel_Sizes(face, 0, fontSize);
        if (e != FT_Err_Ok) { std::cout << FT_Error_String(e) << "\r\n"; }
        
        fontCache.insert({font, std::make_unique<FT_Face>(face)});
        return font.get();
    }

    Text::Character RendererBackend::LoadGlyph(char32_t character, Text::Font* font)
    {
        auto characterIterator = font->characters.find(character);
        if (characterIterator != font->characters.end())
            return characterIterator->second;
        
        FT_Face face = nullptr;
        for (auto& [_font, _face] : fontCache) {
            if (font->name == _font->name && font->fontSize == _font->fontSize)
            {
                face = *_face.get();
                break;
            }
        }

        if (!face) {
            std::cerr << "[ERROR] Could not find FT_Face for font " << font->name << " at size " << font->fontSize << std::endl;
            return Text::Character();
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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

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

        if (GLAD_GL_VERSION_4_3) 
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
            glDebugMessageCallback(MessageCallback, nullptr);
            
            // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

            std::cout << "OpenGL Debug Callback enabled successfully" << std::endl;
        } else std::cerr << "OpenGL Debug not supported by current driver" << std::endl;

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
        glDisable(GL_SCISSOR_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererBackend::DrawText(float x, float y, const Rect& clipRect, const std::string text, Text::Font* font, const Data::Material& material)
    {
        glEnable(GL_SCISSOR_TEST);
        int scissorX = clipRect.x;
        int scissorY = screen.height - (clipRect.y + clipRect.height); 
        
        glScissor(scissorX, scissorY, clipRect.width, clipRect.height);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        unsigned int textures[font->fontAtlases.size()];
        glGenTextures(font->fontAtlases.size(), textures);

        for (int i = 0; i < font->fontAtlases.size(); i++) // TODO: move this awful awful font atlas generation code out of the text drawing method like why tf is it here
        {
            glBindTexture(GL_TEXTURE_2D, textures[i]);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                font->atlasSize,
                font->atlasSize,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                font->fontAtlases[i].atlasTexture->pixels.data()
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        auto* sp = static_cast<ShaderProgram*>(material.shader.get());
        sp->Use();
        sp->SetMatrix4x4("projection", glm::value_ptr(glm::ortho(0.0f, (float)screen.width, (float)screen.height, 0.0f, -100.0f, 100.0f)));
        sp->SetFloat3("textColor", new float[]{1.0f, 1.0f, 1.0f});
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        int baselineY = screen.height - y;

        for (char c : text)
        {
            Text::Character ch = font->GetCharacter(c);
            float xpos = x + ch.Bearing[0];
            float ypos = baselineY - ch.Bearing[1];

            float w = ch.Size[0];
            float h = ch.Size[1];
            
            float vertices[6][4] = {
                { xpos,     ypos + h,       ch.UV[0], ch.UV[3] },
                { xpos,     ypos,   ch.UV[0], ch.UV[1] },
                { xpos + w, ypos + h,       ch.UV[2], ch.UV[3] },

                { xpos,     ypos,   ch.UV[0], ch.UV[1] },
                { xpos + w, ypos,   ch.UV[2], ch.UV[1] },
                { xpos + w, ypos + h,       ch.UV[2], ch.UV[3] }
            };

            glBindTexture(GL_TEXTURE_2D, textures[ch.atlasPageIndex]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += ch.Advance;
        }

        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteTextures(font->fontAtlases.size(), textures);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_SCISSOR_TEST);
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