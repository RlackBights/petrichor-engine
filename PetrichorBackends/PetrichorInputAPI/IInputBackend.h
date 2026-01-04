#pragma once

#include <cstdint>

namespace PetrichorInputAPI {
    class IInputBackend
    {
    public:
        virtual ~IInputBackend() = default;

        virtual bool IsKeyDown(uint32_t keyCode, uint16_t* keyModifier) const = 0;
        virtual bool IsKeyPressed(uint32_t keyCode, uint16_t* keyModifier) const = 0;
        virtual bool IsKeyReleased(uint32_t keyCode, uint16_t* keyModifier) const = 0;

        virtual float* GetMousePosition() const = 0;
        virtual float* GetMouseDelta() const = 0;
        virtual float GetMouseScroll() const = 0;

        virtual void InitializeInput() = 0;
        virtual void WrapInput() = 0;
        virtual void UpdateInput() = 0;
    };
}
