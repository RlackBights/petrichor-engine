#pragma once

#include "Rendering/IRendererBackend.h"
#include "Rendering/MeshRenderer.h"
#include "Rendering/Quad.h"
#include "Rendering/Triangle.h"
namespace PetrichorEngine::Rendering
{
    class NullRendererBackend : public IRendererBackend
    {
    public:
        bool showDebugMessages = false;

        ~NullRendererBackend() = default;

        void InitializeRenderer() override;

        void InitializeFrame() override;

        void DrawTriangle(const Triangle& triangle) override;
        void DrawQuad(const Quad& quad) override;
        void DrawMesh(const MeshRenderer& meshRenderer) override;

        void WrapFrame() override;
    };
}