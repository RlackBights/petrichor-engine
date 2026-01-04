#include "PetrichorRendererAPI/Renderer.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include "PetrichorRendererAPI/Data/Quad.h"
#include "PetrichorRendererAPI/IRendererBackend.h"
#include "PetrichorRendererAPI/NullRendererBackend.h"
#include <iostream>
#include <memory>

namespace PetrichorRendererAPI
{
    IRendererBackend* Renderer::Get()
    {
        if (!Renderer::_backend) 
        {
            std::cout << "No renderer backend connected, initializing renderless system";
            Renderer::_backend = std::make_unique<NullRendererBackend>();
        }
        return _backend.get();
    }

    void Renderer::AssignBackend(std::unique_ptr<IRendererBackend> _backend)
    {
        Renderer::_backend = std::move(_backend);
    }

    void Renderer::InitializeRenderer() { Get()->InitializeRenderer(); }
    void Renderer::InitializeWindow(const std::string& title, int width, int height) { Get()->InitializeWindow(title, width, height); }
    void Renderer::InitializeFrame() { Get()->InitializeFrame(); }
    void Renderer::WrapFrame() { Get()->WrapFrame(); }
    void Renderer::DrawTriangle(const Data::Triangle &triangle, const Data::Material& material) { Get()->DrawTriangle(triangle, material); }
    void Renderer::DrawQuad(const Data::Quad &quad, const Data::Material& material) { Get()->DrawQuad(quad, material); }
    void Renderer::DrawMesh(const Data::Mesh &mesh, const Data::Material& material) { Get()->DrawMesh(mesh, material); }

    std::unique_ptr<IRendererBackend> Renderer::_backend = nullptr;
}