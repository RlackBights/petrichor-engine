#pragma once

#include "JSON/JSONStructs.h"

namespace PetrichorEngine::JSON {
    class JSON
    {
    private:
        static std::string ReadStringToken(std::string input);
        static int ReadIntToken(std::string input);
        static float ReadFloatToken(std::string input);
        static JSONValue ParseValue(std::vector<JSONToken> tokens, size_t& index);
        static JSONValue ParseObject(std::vector<JSONToken> tokens, size_t& index);
        static JSONValue ParseArray(std::vector<JSONToken> tokens, size_t& index);
    public:
        static JSONValue Parse(std::string input);
        static std::string Stringify(JSONValue input);
        static std::vector<JSONToken> Tokenize(std::string input);
    };
}