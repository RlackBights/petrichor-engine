#include "glm/fwd.hpp"
#include "ptc_json_structs.hpp"
#include "ptc_gui_structs.hpp"
#include <any>
#include <cstdarg>
#include <ptc_console.hpp>
#include <string>

const char* Console::GetColorCode(enum Color color) {
    switch (color) {
    case BLACK:           return "\x1b[30m";
    case RED:             return "\x1b[31m";
    case GREEN:           return "\x1b[32m";
    case YELLOW:          return "\x1b[33m";
    case BLUE:            return "\x1b[34m";
    case MAGENTA:         return "\x1b[35m";
    case CYAN:            return "\x1b[36m";
    case WHITE:           return "\x1b[37m";

    case BRIGHT_BLACK:    return "\x1b[90m";
    case BRIGHT_RED:      return "\x1b[91m";
    case BRIGHT_GREEN:    return "\x1b[92m";
    case BRIGHT_YELLOW:   return "\x1b[93m";
    case BRIGHT_BLUE:     return "\x1b[94m";
    case BRIGHT_MAGENTA:  return "\x1b[95m";
    case BRIGHT_CYAN:     return "\x1b[96m";
    case BRIGHT_WHITE:    return "\x1b[97m";

    case BACKGROUND_BLACK:    return "\x1b[40m";
    case BACKGROUND_YELLOW:   return "\x1b[43m";
    case BACKGROUND_MAGENTA:  return "\x1b[45m";
    case BACKGROUND_CYAN:     return "\x1b[46m";
    case BACKGROUND_WHITE:    return "\x1b[47m";

    case BACKGROUND_BRIGHT_BLACK:    return "\x1b[100m";
    case BACKGROUND_BRIGHT_RED:      return "\x1b[101m";
    case BACKGROUND_BRIGHT_GREEN:    return "\x1b[102m";
    case BACKGROUND_BRIGHT_YELLOW:   return "\x1b[103m";
    case BACKGROUND_BRIGHT_BLUE:     return "\x1b[104m";
    case BACKGROUND_BRIGHT_MAGENTA:  return "\x1b[105m";
    case BACKGROUND_BRIGHT_CYAN:     return "\x1b[106m";
    case BACKGROUND_BRIGHT_WHITE:    return "\x1b[107m";

    case BOLD:             return "\x1b[1m";
    case UNDERLINE:        return "\x1b[4m";

    case RESET:            return "\x1b[0m";
    case NOTHING:          return "";

    default:               return "\x1b[0m";
    }
}
void Console::ClearScreen()
{
    printf("\033[2J");
    Console::SetCursorPosition(0, 0);
}
void Console::SetCursorPosition(short x, short y)
{
    #ifdef _WIN32
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
    #else
        printf("\033[%d;%dH", y + 2, x + 1);
    #endif
}
void Console::ClearFormatting()
{
    printf("\x1b[0m");
}
std::string Console::FormatString(const char* format, ...) {
    int size = 100;
    char* buffer = new char[size];

    va_list args;
    va_start(args, format);

    int needed = vsnprintf(buffer, size, format, args);

    if (needed >= size) {
        size = needed + 1;
        delete[] buffer;
        buffer = new char[size];
        vsnprintf(buffer, size, format, args);
    }

    va_end(args);

    std::string result(buffer);
    delete[] buffer;
    return result;
}
void Console::Write(glm::vec2 val)
{
    Console::Write(Console::FormatString("{ %f, %f }", val.x, val.y));
}
void Console::Write(glm::vec3 val)
{
    Console::Write(Console::FormatString("{ %f, %f, %f }", val.x, val.y, val.z));
}
void Console::Write(glm::vec4 val)
{
    Console::Write(Console::FormatString("{ %f, %f, %f, %f }", val.x, val.y, val.z, val.w));
}
void Console::Write(JSONToken val)
{
    switch (val.first)
    {
        case JSON_LEFT_BRACE:
            Console::Write("\'{\'");
            break;
        case JSON_RIGHT_BRACE:
            Console::Write("\'}\'");
            break;
        case LEFT_BRACKET:
            Console::Write("\'[\'");
            break;
        case JSON_RIGHT_BRACKET:
            Console::Write("\']\'");
            break;
        case JSON_COMMA:
            Console::Write("\',\'");
            break;
        case JSON_COLON:
            Console::Write("\':\'");
            break;
        case JSON_END:
            Console::Write("");
            break;
        case JSON_BOOLEAN:
            Console::Write(std::any_cast<bool>(val.second) ? "true" : "false");
            break;
        case JSON_STRING:
            Console::Write('\"' + std::any_cast<std::string>(val.second) + '\"');
            break;
        case JSON_INT:
            Console::Write(std::to_string(std::any_cast<int>(val.second)));
            break;
        case JSON_FLOAT:
            Console::Write(std::to_string(std::any_cast<float>(val.second)));
            break;
        case JSON_VOID:
            Console::Write("null");
            break;
        }
}
void Console::Write(char val) {
    Write(std::string(1, val));
}
void Console::Write(Rect val) {
    Write(Console::FormatString("{ x: %d, y: %d, width: %d, height: %d }", val.x, val.y, val.width, val.height));
}
void Console::WriteLine(std::string text, Color color, bool continuous) {
    Write(text + '\n', color, continuous);
}
void Console::WriteLine(const char* text, Color color, bool continuous) {
    Write(std::string(text) + '\n', color, continuous);
}
void Console::Write(std::string text, Color color, bool continuous) {
    printf("%s%s%s", GetColorCode(color), text.c_str(), continuous ? "" : "\x1b[0m");
}
void Console::Write(const char* text, Color color, bool continuous) {
    Write(std::string(text));
}