#ifndef PTC_CONSOLE_HPP
#define PTC_CONSOLE_HPP

#include "glm/fwd.hpp"
#include "ptc_json_structs.hpp"
#include "ptc_gui_structs.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <iomanip>
#include <map>
#include <sstream>
#include <utility>
#include <vector>
#undef APIENTRY
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

enum Color {
    // Foreground colors
    BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
    BRIGHT_BLACK, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE,

    // Background colors
    BACKGROUND_BLACK, BACKGROUND_YELLOW, BACKGROUND_MAGENTA, BACKGROUND_CYAN, BACKGROUND_WHITE,
    BACKGROUND_BRIGHT_BLACK, BACKGROUND_BRIGHT_RED, BACKGROUND_BRIGHT_GREEN, BACKGROUND_BRIGHT_YELLOW, BACKGROUND_BRIGHT_BLUE, BACKGROUND_BRIGHT_MAGENTA, BACKGROUND_BRIGHT_CYAN, BACKGROUND_BRIGHT_WHITE,

    // Attributes (for text formatting)
    BOLD, UNDERLINE, RESET, NOTHING
};

class Console
{
private:
    static const char* GetColorCode(enum Color color);
public:
    static void ClearScreen();
    static void SetCursorPosition(short x, short y);
    static void ClearFormatting();
    static std::string FormatString(const char* format, ...);
    template <class T>
    static void WriteLine(T val)
    {
        Write(val);
        Write("\n");
    }
    template <class T>
    static void Write(T val)
    {
        printf("%s", ToString(val).c_str());
        std::fflush(stdout);
    }
    template <class T>
    static std::string ToString(T val)
    {
        return std::to_string(val);
    }
    template <class T>
    static std::string ToString(std::vector<T> val)
    {
        std::string out("[ ");
        if (val.size() == 0) out += ']';
        else for (size_t i = 0; i < val.size(); i++) { out += ToString(val[i]) + ((i == val.size() - 1) ? " ]" : ", "); }
        return out;
    }
    template <class T>
    static std::string ToString(T* val)
    {
        return ToString(*val);
    }
    template <class T1, class T2>
    static std::string ToString(std::map<T1, T2> val)
    {
        std::string out("{ ");
        int c = val.size();
        for (std::pair<T1, T2> pair : val) {
            out += ToString(pair.first) + ": " + ToString(pair.second) + (--c == 0 ? " }" : ", ");
        }
        return out;
    }
    template< typename T >
    static std::string ToHex( T i )
    {
        std::stringstream stream;
        stream << "0x" 
                << std::setfill ('0') << std::setw(sizeof(T)*2) 
                << std::hex << i;
        return stream.str();
    }
    static std::string ToString(const char* val);
    static std::string ToString(std::string& val);
    static std::string ToString(glm::vec2& val);
    static std::string ToString(glm::vec3& val);
    static std::string ToString(glm::vec4& val);
    static std::string ToString(JSONToken val);
    static std::string ToString(JSONValue val);
    static std::string ToString(char val);
    static std::string ToString(Rect val);
    static void WriteLine(std::string text, Color color, bool continuous = true);
    static void WriteLine(const char* text, Color color, bool continuous = true);
    static void Write(std::string text, Color color, bool continuous = true);
    static void Write(const char* text, Color color, bool continuous = true);
};

#endif