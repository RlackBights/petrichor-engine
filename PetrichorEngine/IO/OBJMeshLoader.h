#pragma once

#include "IO/IMeshLoaderBase.h"
#include "IO/MeshHierarchy.h"
#include <string>

namespace PetrichorEngine::IO {
    struct OBJFaceToken
    {
        int v = 0;
        int vt = 0;
        int vn = 0;

        OBJFaceToken(const std::string& token);
    };

    class OBJMeshLoader : public IMeshLoaderBase
    {
    public:
        ~OBJMeshLoader() = default;

        IO::MeshHierarchy GenerateMeshHierarchy(const std::string& path) override;
    };
}