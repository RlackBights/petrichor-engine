#include "PetrichorInputAPI/NullInputBackend.h"

namespace PetrichorInputAPI {
    bool NullInputBackend::IsKeyDown(uint32_t keyCode, uint16_t* keyModifier) const { return false; }
    bool NullInputBackend::IsKeyPressed(uint32_t keyCode, uint16_t* keyModifier) const { return false; }
    bool NullInputBackend::IsKeyReleased(uint32_t keyCode, uint16_t* keyModifier) const { return false; }

    float* NullInputBackend::GetMousePosition() const { return nullptr; }
    float* NullInputBackend::GetMouseDelta() const { return nullptr; }
    float NullInputBackend::GetMouseScroll() const { return 0; }

    void NullInputBackend::InitializeInput() {}
    void NullInputBackend::WrapInput() {}
    void NullInputBackend::UpdateInput() {}
}