#include <ptc_component.h>

Object* Component::GetParentObject() { return parentObject; }
void Component::SetParentObject(Object* object) { parentObject = object; }