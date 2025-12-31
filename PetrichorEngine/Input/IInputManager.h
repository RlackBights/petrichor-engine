#pragma once

#include "Math/Math.h"
#include <cstdint>

namespace PetrichorEngine::Input {
    class IInputManager
    {
    public:
        virtual ~IInputManager() = default;

        virtual bool IsKeyDown(uint32_t keyCode) const = 0;
        virtual bool IsKeyPressed(uint32_t keyCode) const = 0;
        virtual bool IsKeyReleased(uint32_t keyCode) const = 0;

        virtual Math::Vector2 GetMousePosition() const = 0;
        virtual Math::Vector2 GetMouseDelta() const = 0;
        virtual float GetMouseScroll() const = 0;

        virtual void Update() = 0;
    };
}
