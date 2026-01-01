#pragma once

#include "ECS/Component.h"
#include "Rendering/Mesh.h"
#include <memory>

namespace PetrichorEngine::Rendering {
    class MeshFilter : public ECS::Component
    {
    public:
        std::shared_ptr<Mesh> mesh;

        MeshFilter();
    };
}