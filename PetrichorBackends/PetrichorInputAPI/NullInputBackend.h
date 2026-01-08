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

        bool IsMouseButtonDown(uint32_t keyCode) override;
        bool IsMouseButtonPressed(uint32_t keyCode) override;
        bool IsMouseButtonReleased(uint32_t keyCode) override;

        float* GetMousePosition() const override;
        float* GetMouseDelta() const override;
        float GetMouseScroll() const override;
        const char* GetLastCharacter() override;

        uint32_t GetLastKey() override;
        uint32_t GetLastKeyDown() override;

        void InitializeInput() override;
        void WrapInput() override;
        void UpdateInput() override;
    };
}