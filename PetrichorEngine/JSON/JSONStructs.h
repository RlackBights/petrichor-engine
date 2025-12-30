#pragma once

#include <any>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace PetrichorEngine {
    struct JSONValue;
    using JSONArray = std::vector<JSONValue>;
    using JSONObject = std::map<std::string, JSONValue>;

    struct JSONValue : std::variant<
        JSONArray,
        JSONObject,
        std::string,
        int,
        float,
        bool
    > {
        using variant::variant;

        friend std::ostream& operator<<(std::ostream& stream, const JSONValue value)
        {
            stream << "[JSONValue]";
            return stream;
        }
    };

    enum JSONTokenType
    {
        JSON_LEFT_BRACE,
        JSON_RIGHT_BRACE,
        LEFT_BRACKET,
        JSON_RIGHT_BRACKET,
        JSON_COMMA,
        JSON_COLON,
        JSON_END,
        JSON_STRING,
        JSON_INT,
        JSON_FLOAT,
        JSON_BOOLEAN,
        JSON_VOID
    };

    struct JSONToken : std::pair<JSONTokenType, std::any>
    {
        using pair::pair;

        friend std::ostream& operator<<(std::ostream& stream, const JSONToken value)
        {
            stream << "[JSONToken]";
            return stream;
        }
    };
}

