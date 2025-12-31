#pragma once

#include "ECS/Component.h"

namespace PetrichorEngine::Rendering {
    using MeshReference = uint32_t;

    struct MeshFilter : public ECS::Component
    {
    public:
        MeshReference mesh;
    };
}
