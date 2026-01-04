#pragma once

#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include "PetrichorRendererAPI/Data/Triangle.h"
#include "PetrichorRendererAPI/IRendererBackend.h"
#include "PetrichorRendererAPI/Text/FontLoader.h"
#include "PetrichorRendererAPI/Text/FontLoader.h"
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
        static void InitializeRenderer();
        static void InitializeWindow(const std::string& title = "PetrichorGame Window", int width = 800, int height = 600);

        static void InitializeFrame();

        static void DrawTriangle(const Data::Triangle& triangle, const Data::Material& material);
        static void DrawQuad(const Data::Quad& quad, const Data::Material& material);
        static void DrawMesh(const Data::Mesh& mesh, const Data::Material& material);

        static void WrapFrame();
    };
}