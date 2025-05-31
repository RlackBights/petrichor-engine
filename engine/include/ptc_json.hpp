#ifndef PTC_JSON_HPP
#define PTC_JSON_HPP

#include "ptc_component.hpp"
#include "ptc_json_structs.hpp"
#include <string>
#include <vector>

class JSON : public Component
{
private:
    static std::string ReadStringToken(std::string input);
    static int ReadIntToken(std::string input);
    static float ReadFloatToken(std::string input);
    static JSONValue ParseValue(std::vector<JSONToken> tokens, size_t index);
    static JSONValue ParseObject(std::vector<JSONToken> tokens, size_t index);
    static JSONValue ParseArray(std::vector<JSONToken> tokens, size_t index);
public:
    static JSONValue Parse(std::string input);
    static std::vector<JSONToken> Tokenize(std::string input);
};

#endif