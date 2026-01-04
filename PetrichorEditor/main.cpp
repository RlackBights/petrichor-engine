#include "PetrichorInputAPI/InputManager.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "PetrichorRendererAPI/Data/Quad.h"
#include "PetrichorRendererAPI/Renderer.h"
#include "SDL3_OpenGL_Backend/InputBackend.h"
#include "SDL3_OpenGL_Backend/RendererBackend.h"
#include <Core/Log.h>
#include <memory>
#include <unistd.h>
#include <utility>

int main()
{
    auto rendererBackend = std::make_unique<RendererBackends::SDL3_OpenGL::RendererBackend>();
    PetrichorRendererAPI::Renderer::AssignBackend(std::move(rendererBackend));

    auto inputBackend = std::make_unique<RendererBackends::SDL3_OpenGL::InputBackend>();
    PetrichorInputAPI::InputManager::AssignInputManager(std::move(inputBackend));

    PetrichorRendererAPI::Renderer::InitializeWindow("Game!!!!");
    PetrichorInputAPI::InputManager::InitializeInput();

    PetrichorRendererAPI::Data::Quad q;
    PetrichorRendererAPI::Data::Material m;

    while (1)
    {
        // PetrichorRendererAPI::Renderer::InitializeFrame();
        PetrichorInputAPI::InputManager::UpdateInput();

        PetrichorRendererAPI::Renderer::DrawQuad(q, m);

        PetrichorInputAPI::InputManager::WrapInput();
        // PetrichorRendererAPI::Renderer::WrapFrame();
    }

    return 0;
}