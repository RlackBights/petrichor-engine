#include "IO/MeshLoader.h"
#include "Core/Log.h"
#include "ECS/Object.h"
#include "IO/MeshHierarchy.h"
#include "IO/OBJMeshLoader.h"
#include "Rendering/MeshFilter.h"
#include <filesystem>
#include <functional>

namespace PetrichorEngine::IO {
    ModelType MeshLoader::GetModelType(const std::string& path)
    {
        std::string extension = std::filesystem::path(path).extension().string();
        
        if (extension == ".obj") return ModelType::OBJ;
        else return ModelType::NONE;
    }

    ECS::Object* MeshLoader::ParseMeshHierarchy(const MeshHierarchy& hierarchy)
    {
        Core::Log::Error("MESH HIERARCHY PARSING NOT IMPLEMENTED YET");

        std::function<void(MeshHierarchy, ECS::Object*)> preorder = [&](MeshHierarchy sub, ECS::Object* parent) {
            ECS::Object obj = ECS::Object(sub.mesh->name);
            obj.AddComponent<Rendering::MeshFilter>();

        };

        return ECS::Object::Find(hierarchy.mesh->name);
    }

    ECS::Object* MeshLoader::LoadMesh(const std::string &path)
    {
        MeshHierarchy hierarchy;
        switch (GetModelType(path)) {
            case ModelType::OBJ:
                hierarchy = OBJMeshLoader().GenerateMeshHierarchy(path);
                break;
            case ModelType::NONE:
            default:
                Core::Log::Error("Invalid model type found in path: " + path);
                return {};
        }

        return ParseMeshHierarchy(hierarchy);
    }
}