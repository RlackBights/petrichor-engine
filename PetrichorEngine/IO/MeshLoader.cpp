#include "IO/MeshLoader.h"
#include "Core/Log.h"
#include "ECS/Object.h"
#include "IO/MeshHierarchy.h"
#include "IO/OBJMeshLoader.h"
#include "PetrichorRendererAPI/Data/Material.h"
#include "Rendering/MeshFilter.h"
#include "Rendering/MeshRenderer.h"
#include <filesystem>
#include <functional>
#include <memory>

namespace PetrichorEngine::IO {
    ModelType MeshLoader::GetModelType(const std::string& path)
    {
        std::string extension = std::filesystem::path(path).extension().string();
        
        if (extension == ".obj") return ModelType::OBJ;
        else return ModelType::NONE;
    }

    ECS::Object* MeshLoader::ParseMeshHierarchy(const MeshHierarchy& hierarchy)
    {
        std::function<void(const MeshHierarchy&, ECS::Object*)> preorder = [&](const MeshHierarchy& sub, ECS::Object* parent) {
            ECS::Object obj = ECS::Object(sub.mesh->name);
            obj.transform->parent = (parent ? parent->transform.get() : nullptr);

            auto filterRef = obj.AddComponent<Rendering::MeshFilter>();
            filterRef->mesh = sub.mesh;

            auto rendererRef = obj.AddComponent<Rendering::MeshRenderer>();
            rendererRef->material = std::make_shared<PetrichorRendererAPI::Data::Material>();

            for (auto& child : sub.children) preorder(child, &obj);
        };

        preorder(hierarchy, nullptr);
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