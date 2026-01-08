#include "PetrichorInputAPI/InputManager.h"
#include "PetrichorInputAPI/NullInputBackend.h"
#include <memory>
#include <utility>

namespace PetrichorInputAPI {
    IInputBackend* InputManager::Get()
    {
        if (!_backend) _backend = std::make_unique<NullInputBackend>();
        return _backend.get();
    }

    void InputManager::Enable()
    {
        enabled = true;
    }

    void InputManager::Disable()
    {
        enabled = false;
    }
    
    void InputManager::AssignInputManager(std::unique_ptr<IInputBackend> backend)
    {
        InputManager::_backend = std::move(backend);
    }
    
    bool InputManager::IsKeyDown(uint32_t keyCode, uint16_t* keyModifier)
    {
        return Get()->IsKeyDown(keyCode, keyModifier);
    }

    bool InputManager::IsKeyPressed(uint32_t keyCode, uint16_t* keyModifier)
    {
        return Get()->IsKeyPressed(keyCode, keyModifier);
    }
    bool InputManager::IsKeyReleased(uint32_t keyCode, uint16_t* keyModifier)
    {
        return Get()->IsKeyReleased(keyCode, keyModifier);
    }

    bool InputManager::IsMouseButtonDown(uint32_t buttonCode)
    {
        return Get()->IsMouseButtonDown(buttonCode);
    }
    bool InputManager::IsMouseButtonPressed(uint32_t buttonCode)
    {
        return Get()->IsMouseButtonPressed(buttonCode);
    }
    bool InputManager::IsMouseButtonReleased(uint32_t buttonCode)
    {
        return Get()->IsMouseButtonReleased(buttonCode);
    }

    float* InputManager::GetMousePosition()
    {
        return Get()->GetMousePosition();
    }

    float* InputManager::GetMouseDelta()
    {
        return Get()->GetMouseDelta();
    }

    float InputManager::GetMouseScroll()
    {
        return Get()->GetMouseScroll();
    }

    void InputManager::InitializeInput()
    {
        Get()->InitializeInput();
    }

    void InputManager::UpdateInput()
    {
        if (!enabled) return;
        Get()->UpdateInput();
    }

    void InputManager::WrapInput()
    {
        if (!enabled) return;
        Get()->WrapInput();
    }

    const char* InputManager::GetLastCharacter() { return Get()->GetLastCharacter(); }
    uint32_t InputManager::GetLastKey() { return Get()->GetLastKey(); }
    uint32_t InputManager::GetLastKeyDown() { return Get()->GetLastKeyDown(); }

    std::unique_ptr<IInputBackend> InputManager::_backend;
    bool InputManager::enabled = true;
}