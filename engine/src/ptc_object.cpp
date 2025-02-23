#include <ptc_object.h>
#include <ptc_component.h>
Object::Object(std::string _name, Transform _transform, bool _enabled) : name(_name), transform(_transform), enabled(_enabled) {}
