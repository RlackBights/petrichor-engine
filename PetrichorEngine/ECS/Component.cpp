#include "Component.h"

namespace PetrichorEngine {
    Object* Component::GetParentObject() { return parentObject; }
    void Component::SetParentObject(Object* object) { parentObject = object; }
}