#include "ptc_renderer.h"
#include "ptc_text.h"
#include "ptc_time.h"
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <ptc_component.h>

class WelcomeTextManager : public Component
{
private:
    Text* textRef;
    void FixedUpdate() override
    {
        textRef->MoveText(Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f);
        textRef->animationFunction = [](float _x) -> float { return (int)(glm::sin(_x + Time::time * 10) * 20.0f) % 3; };
    }
public:
    void Start() override
    {
        textRef = parentObject->AddComponent<Text>("Welcome...", Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, Font::LoadFont("asd.ttf", 48), glm::vec4(1.0f, 0.1f, 0.1f, 1.0f)); 
        textRef->CenterText();
        Time::createTimer(3.0f, [&]() { textRef->SetText("This will be a [00ff00]simple[ff1a1a] game,\ndon't worry"); });
        Time::createTimer(6.0f, [&]() { textRef->SetText("Type the following words\n\n\n\n\nExperiment a little"); });
    }
};