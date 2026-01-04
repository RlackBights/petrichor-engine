#pragma once

#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/IRendererBackend.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Quad.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Triangle.h"

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

        void InitializeWindow(const std::string& title = "", int width = -1, int height = -1) override;

        void InitializeRenderer() override;

        void InitializeFrame() override;

        void DrawTriangle(const Data::Triangle& triangle, const Data::Material& material) override;
        void DrawQuad(const Data::Quad& quad, const Data::Material& material) override;
        void DrawMesh(const Data::Mesh& mesh, const Data::Material& material) override;

        void WrapFrame() override;

    };
}