#ifndef PTC_CONSOLE_HPP
#define PTC_CONSOLE_HPP

#include "glm/fwd.hpp"
#include "ptc_json_structs.hpp"
#include "ptc_gui_structs.hpp"
#include <cstddef>
#include <vector>
#undef APIENTRY
#include <string>

#ifdef _WIN32
    #include <Windows.h>
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
    static void WriteLine(std::vector<T> val)
    {
        Write("[");
        Write(val);
        WriteLine("]");
    }
    template <class T>
    static void Write(T val)
    {
        Write(std::to_string(val));
    }
    template <class T>
    static void Write(std::vector<T> val)
    {
        for (size_t i = 0; i < val.size(); i++) { Write(val[i]); Write((i == val.size() - 1) ? "" : ", "); }
    }
    static void Write(glm::vec2 val);
    static void Write(glm::vec3 val);
    static void Write(glm::vec4 val);
    static void Write(JSONToken val);
    static void Write(char val);
    static void Write(Rect val);
    static void WriteLine(std::string text, Color color = Color::NOTHING, bool continuous = true);
    static void WriteLine(const char* text, Color color = Color::NOTHING, bool continuous = true);
    static void Write(std::string text, Color color = Color::NOTHING, bool continuous = true);
    static void Write(const char* text, Color color = Color::NOTHING, bool continuous = true);
};

#endif