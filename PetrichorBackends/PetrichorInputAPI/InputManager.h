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

        static float* GetMousePosition();
        static float* GetMouseDelta();
        static float GetMouseScroll();

        static void InitializeInput();
        static void UpdateInput();
        static void WrapInput();
    };
}