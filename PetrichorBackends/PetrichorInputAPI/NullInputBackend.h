#pragma once

#include "PetrichorInputAPI/IInputBackend.h"

namespace PetrichorInputAPI {
    class NullInputBackend : public IInputBackend
    {
    public:
        ~NullInputBackend() = default;

        bool IsKeyDown(uint32_t keyCode, uint16_t* keyModifier) const override;
        bool IsKeyPressed(uint32_t keyCode, uint16_t* keyModifier) const override;
        bool IsKeyReleased(uint32_t keyCode, uint16_t* keyModifier) const override;

        float* GetMousePosition() const override;
        float* GetMouseDelta() const override;
        float GetMouseScroll() const override;

        void InitializeInput() override;
        void WrapInput() override;
        void UpdateInput() override;
    };
}