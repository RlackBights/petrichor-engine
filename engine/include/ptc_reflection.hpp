#ifndef PTC_REFLECTION_HPP
#define PTC_REFLECTION_HPP

#include "ptc_object.hpp"
#include <string>
class Reflection
{
private:
    
public:
    static std::string SerializeObject(Object& object);
    static Object DeserializeObject(std::string& src);
};

#endif