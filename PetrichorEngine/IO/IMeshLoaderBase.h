#pragma once

#include "IO/MeshHierarchy.h"
#include <string>

namespace PetrichorEngine::IO {
    class IMeshLoaderBase
    {
    public:
        virtual ~IMeshLoaderBase() = default;

        virtual IO::MeshHierarchy GenerateMeshHierarchy(const std::string& path) = 0;
    };
}