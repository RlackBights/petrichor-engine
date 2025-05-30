#include <SDL3/SDL_events.h>
#include <SDL3/SDL_stdinc.h>
#include <ptc_input.hpp>

 void Input::initInput(int* inScreenWidth, int* inScreenHeight, float mouseSensitivity) {
    sensitivity = mouseSensitivity;
    screenWidth = inScreenWidth;
    screenHeight = inScreenHeight;
}
 void Input::updateInputUnscaled() {
    SDL_PollEvent(&e);

    // Handle key events
    if (e.type == SDL_EVENT_KEY_DOWN) {
        lastKey = e.key.key;
        heldKeys[e.key.scancode] = true;
    }
    else if (e.type == SDL_EVENT_KEY_UP) {
        heldKeys[e.key.scancode] = false;
        lastKeys[e.key.scancode] = false;
    }

    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        heldMouseButtons[e.button.button] = true;
    }
    else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        heldMouseButtons[e.button.button] = false;
        lastMouseButtons[e.button.button] = false;
    }

    // Process forced key bindings
    for (int i = 0; i < keyBindingsForced.size(); i++) {
        switch (keyBindingsForced[i].eventType) {
        case KEYBIND_DOWN:
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == keyBindingsForced[i].key &&
                lastKeys[e.key.scancode] != heldKeys[e.key.scancode]
                ) {
                lastKeys[e.key.scancode] = heldKeys[e.key.scancode];
                keyBindingsForced[i].action();
            }
            break;
        case KEYBIND_UP:
            if (e.type == SDL_EVENT_KEY_UP && e.key.key == keyBindingsForced[i].key) {
                keyBindingsForced[i].action();
            }
            break;
        case KEYBIND_HOLD:
            if (heldKeys[SDL_GetScancodeFromKey(keyBindingsForced[i].key, NULL)]) {
                keyBindingsForced[i].action();
            }
            break;
        case MOUSE_DOWN:
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == keyBindingsForced[i].key &&
                lastMouseButtons[e.button.button] != heldMouseButtons[e.button.button]
                ) {
                lastMouseButtons[e.button.button] = heldMouseButtons[e.button.button];
                keyBindingsForced[i].action();
            }
            break;
        case MOUSE_UP:
            if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == keyBindingsForced[i].key) {
                keyBindingsForced[i].action();
            }
            break;
        case MOUSE_HOLD:
            if (heldMouseButtons[keyBindingsForced[i].key]) {
                keyBindingsForced[i].action();
            }
            break;
        default:
            break;
        }
    }

    if (e.type == SDL_EVENT_QUIT) exit(0);

    if (e.type == SDL_EVENT_WINDOW_RESIZED)
    {
        SDL_GetWindowSizeInPixels(SDL_GetWindowFromEvent(&e), screenWidth, screenHeight);
    }

    if (!enabled) return;

    if (e.type == SDL_EVENT_MOUSE_WHEEL) mouseScroll += e.wheel.y;

    if (e.type == SDL_EVENT_MOUSE_MOTION) {
        mouseXrel += e.motion.xrel * sensitivity;
        mouseYrel += -e.motion.yrel * sensitivity;
        mouseX = e.motion.x;
        mouseY = e.motion.y;
    }

    // Loop through all keybindings and trigger the action
    for (int i = 0; i < keyBindings.size(); i++) {
        switch (keyBindings[i].eventType) {
        case KEYBIND_DOWN:
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == keyBindings[i].key &&
                lastKeys[e.key.scancode] != heldKeys[e.key.scancode]
            ) {
                lastKeys[e.key.scancode] = heldKeys[e.key.scancode];
                keyBindings[i].action();
            }
            break;
        case KEYBIND_UP:
            if (e.type == SDL_EVENT_KEY_UP && e.key.key == keyBindings[i].key) {
                keyBindings[i].action();
            }
            break;
        case KEYBIND_HOLD:
            if (heldKeys[SDL_GetScancodeFromKey(keyBindings[i].key, NULL)]) {
                keyBindings[i].action();
            }
            break;
        case MOUSE_DOWN:
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == keyBindings[i].key &&
                lastMouseButtons[e.button.button] != heldMouseButtons[e.button.button]
                ) {
                lastMouseButtons[e.button.button] = heldMouseButtons[e.button.button];
                keyBindings[i].action();
            }
            break;
        case MOUSE_UP:
            if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == keyBindings[i].key) {
                keyBindings[i].action();
            }
            break;
        case MOUSE_HOLD:
            if (heldMouseButtons[keyBindings[i].key]) {
                keyBindings[i].action();
            }
            break;
        default:
            break;
        }
    }
}
 void Input::wrapInput()
{
    mouseXrel = 0;
    mouseYrel = 0;
    mouseScroll = 0;
    lastKey = 0;
}
 void Input::addBinding(Uint32 key, KeyBindingEventType eventType, std::function<void()> action, bool isForced) {
    if (isForced) keyBindingsForced.push_back(KeyBinding{ key, eventType, action});
    else keyBindings.push_back(KeyBinding{ key, eventType, action });
}
bool Input::getKey(uint _keyCode, ushort* _keyMod)
{
    return heldKeys[SDL_GetScancodeFromKey(_keyCode, _keyMod)];
}

bool Input::enabled;
bool Input::heldKeys[SDL_SCANCODE_COUNT];
bool Input::lastKeys[SDL_SCANCODE_COUNT];
bool Input::heldMouseButtons[256];
bool Input::lastMouseButtons[256];

float Input::sensitivity;
float Input::mouseXrel;
float Input::mouseYrel;
float Input::mouseX;
float Input::mouseY;
float Input::mouseScroll;
Uint32 Input::lastKey;

int* Input::screenWidth;
int* Input::screenHeight;

SDL_Event Input::e;

std::vector<KeyBinding> Input::keyBindings;
std::vector<KeyBinding> Input::keyBindingsForced;