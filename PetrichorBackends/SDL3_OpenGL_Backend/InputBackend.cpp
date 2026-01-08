#include "SDL3_OpenGL_Backend/InputBackend.h"
#include "PetrichorInputAPI/InputManager.h"
#include "PetrichorInputAPI/Keybinds.h"
#include "SDL3/SDL_keyboard.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
#include <iostream>

namespace RendererBackends::SDL3_OpenGL {
    bool InputBackend::IsKeyDown(uint32_t keyCode, uint16_t* keyModifier) const
    {
        return heldKeys[SDL_GetScancodeFromKey(keyCode, keyModifier)];
    }

    bool InputBackend::IsKeyPressed(uint32_t keyCode, uint16_t* keyModifier) const
    {
        SDL_Scancode c = SDL_GetScancodeFromKey(keyCode, keyModifier);
        return heldKeys[c] && !lastKeys[c];
    }

    bool InputBackend::IsKeyReleased(uint32_t keyCode, uint16_t* keyModifier) const
    {
        SDL_Scancode c = SDL_GetScancodeFromKey(keyCode, keyModifier);
        return !heldKeys[c] && lastKeys[c];
    }

    bool InputBackend::IsMouseButtonDown(uint32_t keyCode)
    {
        return heldMouseButtons[keyCode];
    }

    bool InputBackend::IsMouseButtonPressed(uint32_t keyCode)
    {
        return heldMouseButtons[keyCode] && !lastMouseButtons[keyCode];
    }

    bool InputBackend::IsMouseButtonReleased(uint32_t keyCode)
    {
        return !heldMouseButtons[keyCode] && !lastMouseButtons[keyCode];
    }

    float* InputBackend::GetMousePosition() const
    {
        return new float[]{ mouseX, mouseY };
    }

    float* InputBackend::GetMouseDelta() const
    {
        return new float[]{ mouseXrel, mouseYrel };
    }

    float InputBackend::GetMouseScroll() const
    {
        return mouseScroll;
    }

    const char* InputBackend::GetLastCharacter()
    {
        return lastCharacter;
    }

    uint32_t InputBackend::GetLastKey() { return lastKey; }
    uint32_t InputBackend::GetLastKeyDown() { return lastKeyDown; }

    void InputBackend::InitializeInput()
    {
        
    }

    void InputBackend::WrapInput()
    {
        mouseXrel = 0;
        mouseYrel = 0;
        mouseScroll = 0;
        lastKey = 0;
    }

    void InputBackend::UpdateInput()
    {
        for (const auto& binding : PetrichorInputAPI::KeyBindings::keyBindings) {
            if (PetrichorInputAPI::InputManager::enabled &&
                (binding.eventType == PetrichorInputAPI::KeyBindingEventType::HOLD &&
                heldKeys[SDL_GetScancodeFromKey(binding.key, nullptr)]) || 
                (binding.eventType == PetrichorInputAPI::KeyBindingEventType::HOLD &&
                heldMouseButtons[binding.key])) {
                binding.action();
            }
        }

        for (const auto& binding : PetrichorInputAPI::KeyBindings::forcedKeyBindings) {
            if ((binding.eventType == PetrichorInputAPI::KeyBindingEventType::HOLD &&
                heldKeys[SDL_GetScancodeFromKey(binding.key, nullptr)]) || 
                (binding.eventType == PetrichorInputAPI::KeyBindingEventType::HOLD &&
                heldMouseButtons[binding.key])) {
                binding.action();
            }
        }

        if (e.type == SDL_EVENT_MOUSE_WHEEL) mouseScrollRel = e.wheel.y;
        else mouseScrollRel = 0;

        lastCharacter = "";
        lastKeyDown = 0;

        while (SDL_PollEvent(&e))
        {
            
            if (e.type == SDL_EVENT_MOUSE_MOTION) {
                mouseX = e.motion.x;
                mouseY = e.motion.y;
                if (!PetrichorInputAPI::InputManager::enabled) continue;
                mouseXrel += e.motion.xrel;
                mouseYrel += -e.motion.yrel;
                continue;
            }

            if (e.type == SDL_EVENT_MOUSE_WHEEL && PetrichorInputAPI::InputManager::enabled) 
            {
                mouseScroll += e.wheel.y;
                continue;
            }

            if (e.type == SDL_EVENT_TEXT_INPUT) {
                lastCharacter = e.text.text;
            }

            // Handle key events
            if (e.type == SDL_EVENT_KEY_DOWN) {
                lastKey = e.key.key;
                heldKeys[e.key.scancode] = true;
                lastKeyDown = e.key.key;
            }
            else if (e.type == SDL_EVENT_KEY_UP) {
                heldKeys[e.key.scancode] = false;
                lastKeys[e.key.scancode] = false;
            }

            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                heldMouseButtons[e.button.button] = true;
            }
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                heldMouseButtons[e.button.button] = false;
                lastMouseButtons[e.button.button] = false;
            }

            if (e.type == SDL_EVENT_DROP_FILE) {
                std::cout << e.drop.data;
            }

            // Process forced key bindings
            for (int i = 0; i < PetrichorInputAPI::KeyBindings::forcedKeyBindings.size(); i++) {
                switch (PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].eventType) {
                case PetrichorInputAPI::KeyBindingEventType::DOWN:
                    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].key &&
                        lastKeys[e.key.scancode] != heldKeys[e.key.scancode]
                        ) {
                        lastKeys[e.key.scancode] = heldKeys[e.key.scancode];
                        PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].action();
                    }

                    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].key &&
                        lastMouseButtons[e.button.button] != heldMouseButtons[e.button.button]
                        ) {
                        lastMouseButtons[e.button.button] = heldMouseButtons[e.button.button];
                        PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].action();
                    }
                    break;
                case PetrichorInputAPI::KeyBindingEventType::UP:
                    if (e.type == SDL_EVENT_KEY_UP && e.key.key == PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].key) {
                        PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].action();
                    }

                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].key) {
                        PetrichorInputAPI::KeyBindings::forcedKeyBindings[i].action();
                    }
                    break;
                default:
                    break;
                }
            }

            if (e.type == SDL_EVENT_QUIT) exit(0);

            if (e.type == SDL_EVENT_WINDOW_RESIZED)
            {
                std::cout << "[ERROR] Window resizing not implemented yet!\r\n";
                //SDL_GetWindowSizeInPixels(SDL_GetWindowFromEvent(&e), screenWidth, screenHeight);
            }

            if (!PetrichorInputAPI::InputManager::enabled) return;

            // Loop through all keybindings and trigger the action
            for (int i = 0; i < PetrichorInputAPI::KeyBindings::keyBindings.size(); i++) {
                switch (PetrichorInputAPI::KeyBindings::keyBindings[i].eventType) {
                case PetrichorInputAPI::KeyBindingEventType::DOWN:
                    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == PetrichorInputAPI::KeyBindings::keyBindings[i].key &&
                        lastKeys[e.key.scancode] != heldKeys[e.key.scancode]
                    ) {
                        lastKeys[e.key.scancode] = heldKeys[e.key.scancode];
                        PetrichorInputAPI::KeyBindings::keyBindings[i].action();
                    }

                    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == PetrichorInputAPI::KeyBindings::keyBindings[i].key &&
                        lastMouseButtons[e.button.button] != heldMouseButtons[e.button.button]
                        ) {
                        lastMouseButtons[e.button.button] = heldMouseButtons[e.button.button];
                        PetrichorInputAPI::KeyBindings::keyBindings[i].action();
                    }
                    break;
                case PetrichorInputAPI::KeyBindingEventType::UP:
                    if (e.type == SDL_EVENT_KEY_UP && e.key.key == PetrichorInputAPI::KeyBindings::keyBindings[i].key) {
                        PetrichorInputAPI::KeyBindings::keyBindings[i].action();
                    }

                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == PetrichorInputAPI::KeyBindings::keyBindings[i].key) {
                        PetrichorInputAPI::KeyBindings::keyBindings[i].action();
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
}