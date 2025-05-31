#include "ptc_json.hpp"
#include "ptc_console.hpp"
#include "ptc_json_structs.hpp"
#include <any>
#include <cstddef>
#include <memory>
#include <string>
#include <variant>
#include <vector>

int JSON::ReadIntToken(std::string input)
{
    for (size_t i = 0; i < input.size(); i++) {
        if ((input[i] < '0' || input[i] > '9') && input[i] != '-') 
        {
            return std::stoi(input.substr(0, i));
        }
    }
    return 0;
}

float JSON::ReadFloatToken(std::string input)
{
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] != '.' && (input[i] < '0' || input[i] > '9') && input[i] != '-') 
        {
            return std::stof(input.substr(0, i));
        }
    }
    return 0;
}

std::string JSON::ReadStringToken(std::string input)
{
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\"') 
        {
            return input.substr(0, i);
        } else if (input[i] == '\\')
        {
            i++;
        }
    }
    return "";
}

JSONValue JSON::ParseValue(std::vector<JSONToken> tokens, size_t index)
{
    JSONValue val;
    for (size_t i = index; i < tokens.size(); i ++) {
        
    }
    return val;
}

JSONValue JSON::ParseObject(std::vector<JSONToken> tokens, size_t index)
{
    auto obj = std::make_shared<JSONObject>();

    for (size_t i = index; i < tokens.size(); i++) {
        
    }
    return JSONValue(obj);
}

JSONValue JSON::ParseArray(std::vector<JSONToken> tokens, size_t index)
{

}

JSONValue JSON::Parse(std::string input)
{
    return JSON::ParseValue(JSON::Tokenize(input), 0);
}

std::vector<JSONToken> JSON::Tokenize(std::string input)
{
    Console::WriteLine(input);
    
    std::vector<JSONToken> tokens = {};
    std::any token;

    for (size_t i = 0; i < input.size(); i++) {
        char c = input[i];
        switch (c)
        {
            case '{':
                tokens.push_back(JSONToken(LEFT_BRACE, NULL));
                break;
            case '}':
                tokens.push_back(JSONToken(RIGHT_BRACE, NULL));
                break;
            case '[':
                tokens.push_back(JSONToken(LEFT_BRACKET, NULL));
                break;
            case ']':
                tokens.push_back(JSONToken(RIGHT_BRACKET, NULL));
                break;
            case ',':
                tokens.push_back(JSONToken(COMMA, NULL));
                break;
            case ':':
                tokens.push_back(JSONToken(COLON, NULL));
                break;
            case '\0':
                tokens.push_back(JSONToken(END, NULL));
                break;
            case '\"':
                token = JSON::ReadStringToken(input.substr(i + 1));
                tokens.push_back(JSONToken(STRING, std::any_cast<std::string>(token)));
                i += std::any_cast<std::string>(token).size() + 1;
                break;
            default:
                if ((c >= '0' && c <= '9') || c == '-') {
                    if (input.substr(i, input.substr(i).find(',')).find('.') != std::variant_npos) {
                        token = JSON::ReadFloatToken(input.substr(i));
                        tokens.push_back(JSONToken(FLOAT, std::any_cast<float>(token)));
                        i += std::to_string(std::any_cast<float>(token)).size() - 1;
                    } else {
                        token = JSON::ReadIntToken(input.substr(i));
                        tokens.push_back(JSONToken(INT, std::any_cast<int>(token)));
                        i += std::to_string(std::any_cast<int>(token)).size() - 1;
                    }
                } else if (c == 't' || c == 'f') {
                    tokens.push_back(JSONToken(BOOLEAN, std::any_cast<bool>(c == 't')));
                    i += (c == 't') ? 3 : 4;
                } else if (c == 'n') {
                    tokens.push_back(JSONToken(VOID, NULL));
                    i += 3;
                }
                break;
        }
    }

    Console::WriteLine(tokens);
    return tokens;
}