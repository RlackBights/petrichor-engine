#ifndef PTC_JSON_STRUCTS
#define PTC_JSON_STRUCTS

#include <any>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

struct JSONValue;
using JSONArray = std::vector<JSONValue>;
using JSONObject = std::map<std::string, JSONValue>;

struct JSONValue : std::variant<
    std::shared_ptr<JSONArray>,
    std::shared_ptr<JSONObject>,
    std::string,
    int,
    float,
    bool,
    std::monostate
> {
    using variant::variant;
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
};

#endif