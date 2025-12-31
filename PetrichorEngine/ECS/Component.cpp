#include "Component.h"

namespace PetrichorEngine::ECS {
    Object* Component::GetParentObject() { return parentObject; }
    void Component::SetParentObject(Object* object) { parentObject = object; }
}