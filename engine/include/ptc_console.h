#ifndef PTC_CONSOLE_H
#define PTC_CONSOLE_H

#undef APIENTRY
#include <string>
#include <cstdarg>
#include <cstdio>

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
    static void WriteLine(std::string text, Color color = Color::NOTHING, bool continuous = true);
    static void Write(std::string text, Color color = Color::NOTHING, bool continuous = true);
};

#endif