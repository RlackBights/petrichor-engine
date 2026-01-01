#pragma once

#include "ECS/Component.h"
#include "Rendering/Material.h"
#include "Rendering/MeshFilter.h"
#include <memory>

namespace PetrichorEngine::Rendering {
    class MeshRenderer : public ECS::Component
    {
    private:
        MeshFilter* meshFilter;
    public:
        std::shared_ptr<Material> material;

        MeshRenderer();

        void Start() override;
    };
}

