#include "PetrichorInputAPI/Keybinds.h"
namespace PetrichorInputAPI {
    void KeyBindings::AddBinding(const KeyBinding &binding, bool forced)
    {
        if (forced) forcedKeyBindings.push_back(binding);
        else keyBindings.push_back(binding);
    }

    void KeyBindings::RemoveBinding(const std::string &name)
    {
        for (auto it = keyBindings.begin(); it != keyBindings.end(); it++)
        {
            if (it->name == name)
            {
                keyBindings.erase(it);
                break;
            }
        }

        for (auto it = forcedKeyBindings.begin(); it != forcedKeyBindings.end(); it++)
        {
            if (it->name == name)
            {
                forcedKeyBindings.erase(it);
                break;
            }
        }
    }

    std::vector<KeyBinding> KeyBindings::keyBindings = {};
    std::vector<KeyBinding> KeyBindings::forcedKeyBindings = {};
}