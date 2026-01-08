#pragma once

#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include "PetrichorRendererAPI/Data/Rect.h"
#include "PetrichorRendererAPI/Text/Character.h"
#include "PetrichorRendererAPI/Text/FontLoader.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Quad.h"
#include "PetrichorRendererAPI/Text/Font.h"
#include "PetrichorRendererAPI/Data/Triangle.h"

namespace PetrichorRendererAPI
{
    class IRendererBackend
    {
    friend class Text::FontLoader;
    friend class Text::Font;
    private:
        virtual Text::Font* LoadFont(const std::string& path, int fontSize, int atlasSize) = 0;
        virtual Text::Character LoadGlyph(char32_t character, Text::Font* font) = 0;
    public:
        virtual ~IRendererBackend() = default;

        virtual void InitializeWindow(const std::string& title, int width, int height) = 0;
        virtual Data::Rect GetScreenRect() = 0;
        virtual void InitializeRenderer() = 0;

        virtual void InitializeFrame() = 0;
        virtual void SetCursorIcon(uint32_t cursorID) = 0;

        virtual void DrawText(const std::string text, Text::Font* font, const Data::Material& material) = 0;
        virtual void DrawRect(const Data::Rect& rect, const Data::Material& material) = 0;
        virtual void DrawTriangle(const Data::Triangle& triangle, const Data::Material& material) = 0;
        virtual void DrawQuad(const Data::Quad& quad, const Data::Material& material) = 0;
        virtual void DrawMesh(const Data::Mesh& mesh, const Data::Material& material) = 0;
        
        virtual void WrapFrame() = 0;
    };
}