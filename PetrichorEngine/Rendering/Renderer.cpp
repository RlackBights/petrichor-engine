#include "Rendering/Renderer.h"
#include "Core/Log.h"
#include "Rendering/IRendererBackend.h"
#include "Rendering/NullRendererBackend.h"
#include <memory>

namespace PetrichorEngine::Rendering
{
    IRendererBackend* Renderer::Get()
    {
        if (!_backend) 
        {
            Core::Log::Warn("No renderer backend connected, initializing renderless system");
            _backend = std::make_unique<NullRendererBackend>();
        }
        return _backend.get();
    }

    void Renderer::AssignBackend(std::unique_ptr<IRendererBackend> _backend)
    {
        Renderer::_backend = std::move(_backend);
    }

    void Renderer::InitializeFrame() { Get()->InitializeFrame(); }
    void Renderer::WrapFrame() { Get()->WrapFrame(); }
    void Renderer::DrawTriangle(const Triangle &triangle) { Get()->DrawTriangle(triangle); }
    void Renderer::DrawQuad(const Quad &quad) { Get()->DrawQuad(quad); }
    void Renderer::DrawMesh(const MeshRenderer &meshRenderer) { Get()->DrawMesh(meshRenderer); }

    std::unique_ptr<IRendererBackend> Renderer::_backend = nullptr;
}