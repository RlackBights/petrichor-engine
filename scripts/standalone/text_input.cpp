#ifndef TEXT_INPUT
#define TEXT_INPUT

#include "ptc_component.h"
#include "ptc_input.h"
#include "ptc_text.h"
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <functional>
#include <glm/fwd.hpp>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>

typedef struct InteractiveCharacter
{
    char character;
    bool correct = false;
} InteractiveCharacter;

class TextInput : public Component
{
private:
    static glm::vec4 completeColor;
    glm::vec4 baseColor;
    glm::vec4 disabledColor;
    Text* textRef;
    bool ordered;
    bool strictOrder;
    std::vector<InteractiveCharacter> characters;
    std::string text;
    int length;
    float x, y;
    std::function<void(TextInput* _self)> onComplete;
    
    void FixedUpdate() override
    {
        uint scancode = Input::lastKey;
        if (disableWriting || scancode < SDLK_0 || scancode > SDLK_Z || (scancode > SDLK_9 && scancode < SDLK_A)) return;
        
        bool completeWord = true;
        bool blockRest = false;
        bool failedStrict = true;
        std::string finalText = "";
        for (int i = 0; i < length; i++)
        {
            InteractiveCharacter* c = &characters[i];
            if (((ordered && !blockRest) || !ordered) && !c->correct && c->character == (char)(scancode - ((scancode >= SDLK_A && (Input::getKey(SDLK_LSHIFT) || Input::getKey(SDLK_RSHIFT))) ? 32 : 0)))
            {
                c->correct = true;
                scancode = 0;
                failedStrict = false;
                blockRest = true;
            } else if (!c->correct) {
                completeWord = false;
                blockRest = true;
            }

            finalText += ((c->correct) ? vec4ToHexColor(completeColor) : vec4ToHexColor(baseColor)) + c->character;
        }

        if (failedStrict && strictOrder)
        {
            finalText = "";
            for (int i = 0; i < length; i++)
            {
                InteractiveCharacter* c = &characters[i];
                c->correct = false;
                finalText += vec4ToHexColor(baseColor) + c->character;
            }
        }
        textRef->SetText(finalText);
        if (completeWord) onComplete(this);
    }
public:
    bool disableWriting;

    TextInput(std::string _text, float _x = 0.0f, float _y = 0.0f, glm::vec4 _color = glm::vec4(1.0f), bool _ordered = false, bool _strictOrder = false, std::function<void(TextInput* _self)> _onComplete = [](TextInput* _self) {}) : onComplete(_onComplete), text(_text), x(_x), y(_y), ordered(_ordered), strictOrder((!ordered ? false : _strictOrder)), baseColor(_color), disableWriting(false)
    {
        disabledColor = _color * glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        length = 0;
        for (char c : _text)
        {
            length++;
            characters.push_back({c});
        }
    }

    std::string vec4ToHexColor(const glm::vec4& color) {
        int r = static_cast<int>(color.r * 255.0f);
        int g = static_cast<int>(color.g * 255.0f);
        int b = static_cast<int>(color.b * 255.0f);
    
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(2) << r
           << std::setw(2) << g
           << std::setw(2) << b;
        
        return '[' + ss.str() + ']';
    }
    void Awake() override
    {
        textRef = AddComponent<Text>(vec4ToHexColor((disableWriting) ? disabledColor : baseColor) + text, x, y);
        textRef->CenterText();
        textRef->enabled = false;
        enabled = false;
        Time::createTimer(6.0f, [&]() { textRef->enabled = true; enabled = true; });
    }
    void RefreshText(std::string _text = "")
    {
        characters.clear();
        if (_text == "") _text = text;
        text = _text;
        length = 0;
        for (char c : _text)
        {
            length++;
            characters.push_back({c});
        }
        textRef->SetText(vec4ToHexColor((disableWriting) ? disabledColor : baseColor) + _text);
    }
    std::string GetText()
    {
        return text;
    }
    void SetCompleteFunction(std::function<void(TextInput* _self)> _onComplete)
    {
        onComplete = _onComplete;
    }
    void DisableWriting(bool _disable)
    {
        disableWriting = _disable;
        RefreshText();
    }
};

glm::vec4 TextInput::completeColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

#endif