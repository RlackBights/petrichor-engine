#pragma once

#include "Rendering/Mesh.h"
#include <memory>
#include <vector>
namespace PetrichorEngine::IO {
    struct MeshHierarchy
    {
        MeshHierarchy* parent;
        std::unique_ptr<Rendering::Mesh> mesh;
        std::vector<MeshHierarchy> children;
    };
}