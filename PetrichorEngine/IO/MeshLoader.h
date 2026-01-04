#pragma once

#include "ECS/Object.h"
#include "IO/MeshHierarchy.h"
#include <string>

namespace PetrichorEngine::IO
{
    enum class ModelType {
        OBJ,
        NONE
    };

    class MeshLoader
    {
    private:
        static ModelType GetModelType(const std::string& path);
        static ECS::Object* ParseMeshHierarchy(const MeshHierarchy& hierarchy);
    public:
        static ECS::Object* LoadMesh(const std::string& path);
        static ECS::Object CreateEmptyMesh();
    };
}