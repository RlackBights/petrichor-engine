#include <ptc_object.h>
#include <ptc_component.h>
Object::Object(std::string _name, bool _enabled) : name(_name), transform(this), enabled(_enabled) { }
std::vector<std::unique_ptr<Component>>* Object::GetComponents()
{
    return &components;
}
