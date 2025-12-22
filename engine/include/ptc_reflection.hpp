#ifndef PTC_REFLECTION_HPP
#define PTC_REFLECTION_HPP

#include "ptc_json_structs.hpp"
#include "ptc_object.hpp"
#include <string>
class Reflection
{
private:
    
public:
    static JSONValue SerializeJSONObject(const Object& object);
    static std::string SerializeObject(const Object& object);
    static Object DeserializeJSONObject(const JSONValue& object);
    static Object DeserializeObject(const std::string& src);
};

#endif