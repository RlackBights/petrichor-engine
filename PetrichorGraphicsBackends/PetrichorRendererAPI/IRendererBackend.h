#pragma once

#include "Mesh.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/FontLoader.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "Quad.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "Triangle.h"
namespace PetrichorRendererAPI
{
    class IRendererBackend
    {
    friend class Text::FontLoader;
    friend class Text::Font;
    private:
        virtual Text::Font LoadFont(const std::string& path, int fontSize, int atlasSize) = 0;
        virtual Text::Character LoadGlyph(char32_t character, Text::Font* font) = 0;
    public:
        virtual ~IRendererBackend() = default;

        virtual void InitializeRenderer() = 0;

        virtual void InitializeFrame() = 0;

        virtual void DrawTriangle(const Triangle& triangle) = 0;
        virtual void DrawQuad(const Quad& quad) = 0;
        virtual void DrawMesh(const Mesh& mesh) = 0;
        
        virtual void WrapFrame() = 0;
    };
}