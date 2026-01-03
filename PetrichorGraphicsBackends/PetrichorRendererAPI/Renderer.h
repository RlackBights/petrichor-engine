#pragma once

#include "IRendererBackend.h"
#include "PetrichorRendererAPI/Text/FontLoader.h"
#include "Text/FontLoader.h"
#include <memory>
namespace PetrichorRendererAPI
{
    class Renderer
    {
    friend class Text::FontLoader;
    friend class Text::Font;
    private:
        static std::unique_ptr<IRendererBackend> _backend;
        static IRendererBackend* Get();
    public:
        static void AssignBackend(std::unique_ptr<IRendererBackend> _backend);

        static void InitializeFrame();

        static void DrawTriangle(const Triangle& triangle);
        static void DrawQuad(const Quad& quad);
        static void DrawMesh(const Mesh& mesh);

        static void WrapFrame();
    };
}