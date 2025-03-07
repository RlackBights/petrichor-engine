#include <ptc_transform.h>
#include <ptc_ui_transform.h>
#include <ptc_object.h>
#include <ptc_component.h>

Object::Object(std::string _name, bool _isUI, bool _enabled) : name(_name), transform((_isUI ? new UITransform(this) : new Transform(this))), enabled(_enabled) {}
std::vector<std::unique_ptr<Component>>* Object::GetComponents()
{
    return &components;
}
Object* Object::Find(std::string name)
{
    Object* out = nullptr;
    Transform::GetRoot()->PreorderTraversal([name, &out](Transform* node) {
        if (!out && node->object && node->object->name == name) out = node->object;
    });
    return out;
}