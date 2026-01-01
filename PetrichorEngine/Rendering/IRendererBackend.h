#pragma once

#include "Rendering/MeshRenderer.h"
#include "Rendering/Quad.h"
#include "Rendering/Triangle.h"
namespace PetrichorEngine::Rendering
{
    class IRendererBackend
    {
    public:
        virtual ~IRendererBackend() = default;

        virtual void InitializeRenderer() = 0;

        virtual void InitializeFrame() = 0;

        virtual void DrawTriangle(const Triangle& triangle) = 0;
        virtual void DrawQuad(const Quad& quad) = 0;
        virtual void DrawMesh(const MeshRenderer& meshRenderer) = 0;
        
        virtual void WrapFrame() = 0;
    };
}