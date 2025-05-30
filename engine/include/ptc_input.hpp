#ifndef PTC_INPUT_HPP
#define PTC_INPUT_HPP

#include <SDL3/SDL_stdinc.h>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <functional>
#include <vector>

constexpr Uint8 SDLM_LEFT   = 1; /* Left mouse button */
constexpr Uint8 SDLM_MIDDLE = 2; /* Middle mouse button */
constexpr Uint8 SDLM_RIGHT  = 3; /* Right mouse button */
constexpr Uint8 SDLM_SIDE1  = 4; /* First side button, you can offset it from this on, like SDLM_SIDE1 + 2 => 3rd button */


enum KeyBindingEventType {
    KEYBIND_DOWN = SDL_EVENT_KEY_DOWN,
    KEYBIND_UP = SDL_EVENT_KEY_UP,
    KEYBIND_HOLD,
    MOUSE_DOWN = SDL_EVENT_MOUSE_BUTTON_DOWN,
    MOUSE_UP = SDL_EVENT_MOUSE_BUTTON_UP,
    MOUSE_HOLD,
};

// Define the KeyBinding structure to hold a function and its parameters
typedef struct KeyBinding {
    Uint32 key;                            // Key for binding
    KeyBindingEventType eventType;         // Event type (Down, Up, Hold)
    std::function<void()> action;          // Function to call (using std::function for flexibility)
} KeyBinding;

class Input {
public:
    static bool enabled;
    static bool heldKeys[SDL_SCANCODE_COUNT];
    static bool lastKeys[SDL_SCANCODE_COUNT];
    static bool heldMouseButtons[256];
    static bool lastMouseButtons[256];

    static float sensitivity;
    static float mouseXrel;
    static float mouseYrel;
    static float mouseX;
    static float mouseY;
    static float mouseScroll;
    static Uint32 lastKey;

    static int* screenWidth;
    static int* screenHeight;

    static SDL_Event e;
    
    static std::vector<KeyBinding> keyBindings;
    static std::vector<KeyBinding> keyBindingsForced; // Keybinds that run even when the input is disabled

    static void initInput(int* inScreenWidth, int* inScreenHeight, float mouseSensitivity = 0.2f);
    static void updateInputUnscaled();
    static void wrapInput();
    static bool getKey(uint _keyCode, ushort* _keyMod = NULL);

    // Add a new keybinding
    static void addBinding(Uint32 key, KeyBindingEventType eventType, std::function<void()> action, bool isForced = false);
};

#endif