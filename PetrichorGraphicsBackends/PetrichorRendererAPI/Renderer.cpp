#include "Renderer.h"
#include "IRendererBackend.h"
#include "NullRendererBackend.h"
#include <iostream>
#include <memory>

namespace PetrichorRendererAPI
{
    IRendererBackend* Renderer::Get()
    {
        if (!_backend) 
        {
            std::cout << "No renderer backend connected, initializing renderless system";
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
    void Renderer::DrawMesh(const Mesh &mesh) { Get()->DrawMesh(mesh); }

    std::unique_ptr<IRendererBackend> Renderer::_backend = nullptr;
}