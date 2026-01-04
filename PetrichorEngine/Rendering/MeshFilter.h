#pragma once

#include "ECS/Component.h"
#include "PetrichorRendererAPI/Data/Mesh.h"
#include <memory>

namespace PetrichorEngine::Rendering {
    class MeshFilter : public ECS::Component
    {
    public:
        std::shared_ptr<
        PetrichorRendererAPI::Data::Mesh> mesh;

        MeshFilter();
    };
}