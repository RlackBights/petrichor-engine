#pragma once

#include "IRendererBackend.h"
#include "Mesh.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "Quad.h"
#include "Text/Font.h"
#include "Triangle.h"

namespace PetrichorRendererAPI
{
    class NullRendererBackend : public IRendererBackend
    {
    private:
        Text::Font LoadFont(const std::string& path, int fontSize, int atlasSize) override;
        Text::Character LoadGlyph(char32_t character, Text::Font* font) override;
    public:
        bool showDebugMessages = false;

        ~NullRendererBackend() = default;

        void InitializeRenderer() override;

        void InitializeFrame() override;

        void DrawTriangle(const Triangle& triangle) override;
        void DrawQuad(const Quad& quad) override;
        void DrawMesh(const Mesh& mesh) override;

        void WrapFrame() override;

    };
}