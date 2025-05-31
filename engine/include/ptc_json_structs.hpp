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
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    COLON,
    END,
    STRING,
    INT,
    FLOAT,
    BOOLEAN,
    VOID
};

struct JSONToken : std::pair<JSONTokenType, std::any>
{
    using pair::pair;
};

#endif