#include "ptc_reflection.hpp"
#include "ptc_camera.hpp"
#include "ptc_json.hpp"
#include "ptc_mesh_renderer.hpp"
#include <cstdlib>

JSONValue Reflection::SerializeJSONObject(const Object &object)
{

}

std::string Reflection::SerializeObject(const Object &object)
{
    return JSON::Stringify(Reflection::SerializeObject(object));
}

Object Reflection::DeserializeJSONObject(const JSONValue &object)
{
    //MeshRenderer* mesh = (MeshRenderer*)malloc(sizeof(MeshRenderer));
}

Object Reflection::DeserializeObject(const std::string &src)
{
    return Reflection::DeserializeJSONObject(JSON::Parse(src));
}