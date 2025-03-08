#include "ptc_input.h"
#include "ptc_renderer.h"
#include "ptc_text.h"
#include "ptc_time.h"
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <ptc_component.h>

class WelcomeTextManager : public Component
{
    Text* textRef;
    void Start() override
    {
        textRef = GetComponent<Text>();
        textRef->MoveText(Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f);
        textRef->SetText("Welcome...");
        Time::createTimer(3.0f, [&]() { textRef->SetText("This will be a simple game,\ndon't worry"); });
        Time::createTimer(6.0f, [&]() { textRef->SetText("Type something, see what happens:\n\n"); });
    }
    void FixedUpdate() override
    {
        textRef->MoveText(Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f);
        static float time = 0.0f;
        time += Time::deltaTime;
        textRef->animationFunction = [](float _x) -> float { return (int)(glm::sin(_x + time * 10) * 20.0f) % 3; };
    }
};