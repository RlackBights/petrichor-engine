#pragma once

#include "Rendering/IRendererBackend.h"
#include <memory>
namespace PetrichorEngine::Rendering
{
    class Renderer
    {
    private:
        static std::unique_ptr<IRendererBackend> _backend;
        static IRendererBackend* Get();
    public:
        static void InitializeRenderer(std::unique_ptr<IRendererBackend> _backend);
    };
}