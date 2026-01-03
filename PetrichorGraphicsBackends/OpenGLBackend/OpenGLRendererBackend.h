#pragma once

#include "PetrichorRendererAPI/IRendererBackend.h"
#include "PetrichorRendererAPI/Mesh.h"
#include "PetrichorRendererAPI/Quad.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Triangle.h"
#include "freetype/freetype.h"
#include <map>
#include <string>

namespace RendererBackends {
    using namespace PetrichorRendererAPI;

    class OpenGLRendererBackend : public IRendererBackend
    {
    private:
        std::map<Text::Font*, FT_Face> fontCache = {};
        Text::Font LoadFont(const std::string& path, int fontSize, int atlasSize = 1024) override;
        Text::Character LoadGlyph(char32_t character, Text::Font* font) override;
        Text::Character ProcessGlyph(char32_t character, Text::Font* font, FT_GlyphSlot glyph);
    public:
        ~OpenGLRendererBackend() = default;

        void InitializeRenderer() override;

        void InitializeFrame() override;

        void DrawTriangle(const Triangle& triangle) override;
        void DrawQuad(const Quad& quad) override;
        void DrawMesh(const Mesh& mesh) override;

        void WrapFrame() override;
    };
}