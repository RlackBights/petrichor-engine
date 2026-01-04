#pragma once

#include "PetrichorRendererAPI/Data/Mesh.h"
#include <memory>
#include <vector>
namespace PetrichorEngine::IO {
    struct MeshHierarchy
    {
        MeshHierarchy* parent;
        std::shared_ptr<PetrichorRendererAPI::Data::Mesh> mesh;
        std::vector<MeshHierarchy> children;
    };
}