#include "Rendering/Renderer.h"
#include "Rendering/IRendererBackend.h"
#include "Rendering/NullRendererBackend.h"
#include <memory>

namespace PetrichorEngine::Rendering
{
    IRendererBackend* Renderer::Get()
    {
        if (!_backend) _backend = std::make_unique<NullRendererBackend>();
        return _backend.get();
    }

    void Renderer::InitializeRenderer(std::unique_ptr<IRendererBackend> _backend)
    {
        Renderer::_backend = std::move(_backend);
    }

    std::unique_ptr<IRendererBackend> Renderer::_backend = nullptr;
}