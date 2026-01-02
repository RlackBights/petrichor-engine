#pragma once

#include "Rendering/IRendererBackend.h"
#include "Rendering/MeshRenderer.h"
#include "Rendering/Quad.h"
#include "Rendering/Triangle.h"
#include <memory>
namespace PetrichorEngine::Rendering
{
    class Renderer
    {
    private:
        static std::unique_ptr<IRendererBackend> _backend;
        static IRendererBackend* Get();
    public:
        static void AssignBackend(std::unique_ptr<IRendererBackend> _backend);

        static void InitializeFrame();

        static void DrawTriangle(const Triangle& triangle);
        static void DrawQuad(const Quad& quad);
        static void DrawMesh(const MeshRenderer& meshRenderer);

        static void WrapFrame();
    };
}