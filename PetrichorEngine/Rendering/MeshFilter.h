#pragma once

#include "ECS/Component.h"
#include "PetrichorRendererAPI/Mesh.h"
#include <memory>

namespace PetrichorEngine::Rendering {
    class MeshFilter : public ECS::Component
    {
    public:
        std::shared_ptr<
        PetrichorRendererAPI::Mesh> mesh;

        MeshFilter();
    };
}