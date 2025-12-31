#include "ECS/Object.h"
#include "ECS/Component.h"
#include "Core/Log.h"
#include "Scene/Transform.h"
#include <memory>

namespace PetrichorEngine::ECS
{
    Object::Object(std::string _name, bool _enabled) : name(_name), transform(std::make_unique<Scene::Transform>(this)), enabled(_enabled) {}
    std::vector<std::unique_ptr<Component>>* Object::GetComponents()
    {
        return &components;
    }
    Object* Object::Find(std::string name)
    {
        Object* out = nullptr;
        Scene::Transform::GetRoot()->PreorderTraversal([name, &out](Scene::Transform* node) {
            if (!out && node->object && node->object->name == name) out = node->object;
        });
        if (out == NULL) Core::Log::Error("[ERROR] Could not find object with name " + name);
        return out;
    }
}