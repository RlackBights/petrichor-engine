#pragma once

#include "PetrichorRendererAPI/Mesh.h"
#include <memory>
#include <vector>
namespace PetrichorEngine::IO {
    struct MeshHierarchy
    {
        MeshHierarchy* parent;
        std::shared_ptr<PetrichorRendererAPI::Mesh> mesh;
        std::vector<MeshHierarchy> children;
    };
}