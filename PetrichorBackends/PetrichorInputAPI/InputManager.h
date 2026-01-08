#pragma once

#include "PetrichorInputAPI/IInputBackend.h"
#include <memory>

namespace PetrichorInputAPI {
    class InputManager
    {
    private:
        static std::unique_ptr<IInputBackend> _backend;
        static IInputBackend* Get();
    public:
        static bool enabled;
        static void Enable();
        static void Disable();

        static void AssignInputManager(std::unique_ptr<IInputBackend> backend);
        
        static bool IsKeyDown(uint32_t keyCode, uint16_t* keyModifier);
        static bool IsKeyPressed(uint32_t keyCode, uint16_t* keyModifier);
        static bool IsKeyReleased(uint32_t keyCode, uint16_t* keyModifier);
        
        static bool IsMouseButtonDown(uint32_t buttonCode);
        static bool IsMouseButtonPressed(uint32_t buttonCode);
        static bool IsMouseButtonReleased(uint32_t buttonCode);

        static float* GetMousePosition();
        static float* GetMouseDelta();
        static float GetMouseScroll();

        static const char* GetLastCharacter();
        static uint32_t GetLastKey();
        static uint32_t GetLastKeyDown();

        static void InitializeInput();
        static void UpdateInput();
        static void WrapInput();
    };
}