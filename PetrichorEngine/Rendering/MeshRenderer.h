#pragma once

#include "ECS/Component.h"
#include "Rendering/Material.h"
#include "Rendering/MeshFilter.h"

namespace PetrichorEngine::Rendering {
    struct MeshRenderer : public ECS::Component
    {
    public:
        MeshReference mesh;
        MaterialReference material;
    };
}

