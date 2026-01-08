#pragma once

#include "PetrichorInputAPI/IInputBackend.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <cstdint>

namespace RendererBackends::SDL3_OpenGL {
    class InputBackend : public PetrichorInputAPI::IInputBackend
    {
    private:
        bool heldKeys[SDL_SCANCODE_COUNT];
        bool lastKeys[SDL_SCANCODE_COUNT];
        bool heldMouseButtons[256];
        bool lastMouseButtons[256];
        
        float mouseXrel;
        float mouseYrel;
        float mouseX;
        float mouseY;
        float mouseScrollRel;
        float mouseScroll;

        uint32_t lastKey;
        uint32_t lastKeyDown;
        const char* lastCharacter;
        
        SDL_Event e;
    public:
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