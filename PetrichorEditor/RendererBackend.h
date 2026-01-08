#pragma once

#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/IRendererBackend.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include "PetrichorRendererAPI/Data/Quad.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Triangle.h"
#include "freetype/freetype.h"
#include <map>
#include <string>
#include <SDL3/SDL_video.h>

namespace RendererBackends::SDL3_OpenGL {
    using namespace PetrichorRendererAPI;
    using namespace PetrichorRendererAPI::Data;

    class RendererBackend : public IRendererBackend
    {
    private:
        SDL_WindowFlags flags = 0;
        SDL_Window* window = nullptr;
        SDL_GLContext glContext = nullptr;

        std::map<Text::Font*, FT_Face> fontCache = {};
        Text::Font* LoadFont(const std::string& path, int fontSize, int atlasSize = 1024) override;
        Text::Character LoadGlyph(char32_t character, Text::Font* font) override;
        Text::Character ProcessGlyph(char32_t character, Text::Font* font, FT_GlyphSlot glyph);
    public:
        ~RendererBackend() = default;

        void SetWindowFlags(SDL_WindowFlags flags);
        void InitializeWindow(const std::string& title, int width, int height) override;
        void InitializeRenderer() override;

        void InitializeFrame() override;

        void DrawRect(const Rect& rect, const Material& material) override;
        void DrawTriangle(const Triangle& triangle, const Material& material) override;
        void DrawQuad(const Quad& quad, const Material& material) override;
        void DrawMesh(const Mesh& mesh, const Material& material) override;

        void WrapFrame() override;
    };
}