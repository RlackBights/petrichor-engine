#include "ptc_component.h"
#include "ptc_console.h"
#include "ptc_input.h"
#include "ptc_renderer.h"
#include "ptc_text.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <sys/types.h>
class TextInput : public Component
{
    Text* textRef;
    void Start() override
    {
        textRef = GetComponent<Text>();
        textRef->MoveText(Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f);
        textRef->enabled = false;
        Time::createTimer(6.0f, [&]() { textRef->enabled = true; });
    }
    void FixedUpdate() override
    {
        uint scancode = Input::lastKey;
        if (scancode < SDLK_0 || scancode > SDLK_Z || (scancode > SDLK_9 && scancode < SDLK_A) || !textRef->enabled) return;
        textRef->SetText(textRef->GetText() + (char)(scancode - ((scancode >= SDLK_A && (Input::getKey(SDLK_LSHIFT) || Input::getKey(SDLK_RSHIFT))) ? 32 : 0)));
    }
};