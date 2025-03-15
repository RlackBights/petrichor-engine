#include "ptc_math.h"
#include "ptc_object.h"
#include "ptc_renderer.h"
#include "ptc_text.h"
#include "ptc_time.h"
#include "./score_tracker.cpp"
#include "ptc_transform.h"
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <ptc_component.h>

class WelcomeTextManager : public Component
{
private:
    Text* textRef;
    ScoreTracker* scoreRef;
    int cutsceneState;
    float timeStart;
    bool allowEnd;
    void FixedUpdate() override
    {
        if (allowEnd && scoreRef->GetHealth() >= 0.9999f)
        {
            textRef->SetText("");
            enabled = false;
            return;
        }
        switch (cutsceneState)
        {
            case 0:
                textRef->MoveText(Renderer::screenWidth / 2.0f, (Renderer::screenHeight + textRef->GetFont()->fontSize / 2.0f) - Math::ApplyEasing(Time::time - timeStart, EasingFunction::IN_OUT_QUAD) * (Renderer::screenHeight + textRef->GetFont()->fontSize) / 2.0f);
                break;
            case 1:
                textRef->MoveText(Renderer::screenWidth / 2.0f - Math::ApplyEasing(Time::time - timeStart, EasingFunction::IN_OUT_QUAD) * Renderer::screenWidth / 4.0f, Math::Lerp(parentObject->transform.position.y, Renderer::screenHeight * scoreRef->GetHealth() - textRef->GetFont()->fontSize, 0.1f));
                break;
            default:
                textRef->MoveText(Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f);
                break;
        }
        textRef->SetTextAnimation([](float _x) -> float { return (int)(glm::sin(_x + Time::time * 10) * 20.0f) % 3; });
    }
public:
    void Awake() override
    {
        cutsceneState = 0;
        timeStart = Time::time + 0.0f;
        scoreRef = Object::Find("scoreTracker")->GetComponent<ScoreTracker>();
        textRef = AddComponent<Text>("Welcome...", Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, Font::LoadFont("arial.ttf", 48), glm::vec4(1.0f, 0.1f, 0.1f, 1.0f)); 
        textRef->CenterText();

        Time::createTimer(3.0f, [&]() { textRef->SetText("This will be a [00ff00]simple[ff1a1a] game,\ndon't worry"); });
        Time::createTimer(6.0f, [&]() { textRef->SetText("Type the following words\n\n\n\nExperiment a little"); });
    }
    void AdvanceCutscene()
    {
        timeStart = Time::time;
        cutsceneState++;
    }
    Text* GetTextRef()
    {
        return textRef;
    }
    void SetEnd(bool _end)
    {
        allowEnd = _end;
    }
};