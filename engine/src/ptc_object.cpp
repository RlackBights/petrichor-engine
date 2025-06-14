#include "ptc_console.hpp"
#include "ptc_transform.hpp"
#include <ptc_object.hpp>
#include <ptc_component.hpp>
Object::Object(std::string _name, bool _enabled) : name(_name), transform(this), enabled(_enabled) { }
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
    if (out == NULL) Console::WriteLine(Console::FormatString("[ERROR] Could not find object with name %s", name.c_str()), Color::RED, false);
    return out;
}