#include "ptc_component.h"
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include "ptc_gui.h"
#include "ptc_renderer.h"
#include "ptc_text.h"
#include "./text_input.cpp"
#include "ptc_time.h"

class Level4 : public Component
{
private:
    glm::vec2 velocity;
    Text* textRef;
    TextInput* inputRef;
    int blockedHalf;
    bool isBlocked;
    void FixedUpdate() override
    {
        textRef->MoveText(parentObject->transform.position.x + velocity.x, parentObject->transform.position.y + velocity.y);
        if (parentObject->transform.position.x < textRef->getPixelWidth() / 2.0f || parentObject->transform.position.x > Renderer::screenWidth - textRef->getPixelWidth() / 2.0f) 
        {
            parentObject->transform.position.x = glm::clamp(parentObject->transform.position.x, textRef->getPixelWidth() / 2.0f, Renderer::screenWidth - textRef->getPixelWidth() / 2.0f);
            velocity.x *= -1;
        }
        if (parentObject->transform.position.y < textRef->GetFont()->fontSize / 2.0f || parentObject->transform.position.y > Renderer::screenHeight - textRef->GetFont()->fontSize / 2.0f)
        {
            parentObject->transform.position.y = glm::clamp(parentObject->transform.position.y, textRef->GetFont()->fontSize / 2.0f, Renderer::screenHeight - textRef->GetFont()->fontSize / 2.0f);
            velocity.y *= -1;
        }
        switch (blockedHalf) {
            case 0:
                GUI::Panel(0, 0, 0, Renderer::screenWidth, Renderer::screenHeight / 2.0f, glm::vec4(0.1f, 1.0f, 1.0f, 0.15f));
                inputRef->DisableWriting(parentObject->transform.position.y < Renderer::screenHeight / 2.0f, false);
                if (parentObject->transform.position.y < Renderer::screenHeight / 2.0f && !isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = true;
                } else if (isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = false;
                }
                break;
            case 1:
                GUI::Panel(0, Renderer::screenHeight / 2.0f, 0, Renderer::screenWidth, Renderer::screenHeight / 2.0f, glm::vec4(0.1f, 1.0f, 1.0f, 0.15f));
                inputRef->DisableWriting(parentObject->transform.position.y > Renderer::screenHeight / 2.0f, false);
                if (parentObject->transform.position.y > Renderer::screenHeight / 2.0f && !isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = true;
                } else if (isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = false;
                }
                break;
            case 2:
                GUI::Panel(0, 0, 0, Renderer::screenWidth / 2.0f, Renderer::screenHeight, glm::vec4(0.1f, 1.0f, 1.0f, 0.15f));
                inputRef->DisableWriting(parentObject->transform.position.x < Renderer::screenWidth / 2.0f, false);
                if (parentObject->transform.position.x < Renderer::screenWidth / 2.0f && !isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = true;
                } else if (isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = false;
                }
                break;
            default:
                GUI::Panel(Renderer::screenWidth / 2.0f, 0, 0, Renderer::screenWidth / 2.0f, Renderer::screenHeight, glm::vec4(0.1f, 1.0f, 1.0f, 0.15f));
                inputRef->DisableWriting(parentObject->transform.position.x > Renderer::screenWidth / 2.0f, false);
                if (parentObject->transform.position.x > Renderer::screenWidth / 2.0f && !isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = true;
                } else if (isBlocked)
                {
                    inputRef->RefreshText();
                    isBlocked = false;
                }
                break;
        }
    }
    void SwitchSide()
    {
        Time::createTimer(5.0f, [&]() {
            int newRand = rand() % 4;
            blockedHalf = (newRand == blockedHalf) ? (newRand + (rand() % 3) + 1) % 4 : newRand;
            SwitchSide();
        });
    }
public:
    void Awake() override
    {
        textRef = GetComponent<Text>();
        inputRef = GetComponent<TextInput>();
        inputRef->enabled = true;
        blockedHalf = 0;
        SwitchSide();
        velocity = glm::vec2(8.0f, 8.0f);
    }
};