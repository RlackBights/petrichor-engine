#include "ptc_component.hpp"

Object* Component::GetParentObject() { return parentObject; }
void Component::SetParentObject(Object* object) { parentObject = object; }