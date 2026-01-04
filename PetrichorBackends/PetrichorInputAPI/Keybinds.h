#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace PetrichorInputAPI {
    enum class KeyBindingEventType {
        UP,
        DOWN,
        HOLD
    };

    // Define the KeyBinding structure to hold a function and its parameters
    struct KeyBinding {
        std::string name;
        uint32_t key;                            // Key for binding
        KeyBindingEventType eventType;         // Event type (Down, Up, Hold)
        std::function<void()> action;          // Function to call (using std::function for flexibility)
    };

    class KeyBindings {
    public:
        static std::vector<KeyBinding> keyBindings;
        static std::vector<KeyBinding> forcedKeyBindings; // Keybinds that run even when the input is disabled

        static void AddBinding(const KeyBinding& binding, bool forced = false);
        static void RemoveBinding(const std::string& name);
    };
}