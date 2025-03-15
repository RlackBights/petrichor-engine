#ifndef SCORE_TRACKER
#define SCORE_TRACKER

#include "ptc_component.h"
#include "ptc_console.h"
#include "ptc_gui.h"
#include "ptc_math.h"
#include "ptc_renderer.h"
#include "ptc_text.h"
#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>

class ScoreTracker : public Component
{
private:
    uint wordsFinished;
    uint points;
    bool endCutscene;
    float combo;
    float lastCombo;
    float comboDecay;
    float difficultyScaling;
    float startTime;
    float health;
    float lastHealth;
    Text* textRef;

    glm::vec4 GetComboColor()
    {
        switch ((int)glm::floor(combo))
        {
            case 1:
                return glm::vec4(0.1f, 0.2f, 0.9f, 1.0f);
            case 2:
                return glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);
            case 3:
                return glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            case 4:
                return glm::vec4(1.0f, 0.6f, 0.1f, 1.0f);
            case 5:
                return glm::vec4(1.0f, 0.1f, 0.1f, 1.0f);
            case 6:
                return glm::vec4(1.0f, 0.84f, 0.0f, 1.0f);
            default:
                return glm::vec4(1.0f);
        }
    }
    void FixedUpdate() override
    {
        if (endCutscene)
        {
            difficultyScaling = (Time::time - startTime) / 1500.0f;
            health = glm::clamp(health - Time::deltaTime * difficultyScaling, 0.0f, 1.0f);
            combo = glm::clamp(combo - comboDecay, 1.0f, 10.0f);
        }
        if (endCutscene && startTime == 0.0f) startTime = Time::time;
        lastHealth = Math::Lerp(lastHealth, health, 1.01f - health);
        lastCombo = Math::Lerp(lastCombo, glm::fract(combo) * (Renderer::screenWidth / 3.0f), 0.1f);
        textRef->SetText(Console::FormatString("%d", points));
        textRef->SetTextAnimation([&](float _x) {
            return (glm::sin(_x + Time::time * combo) * combo);
        });
        GUI::Panel(0, 0, -1, Renderer::screenWidth, lastHealth * Renderer::screenHeight, glm::vec4(0.2f, 0.8f, 0.3f, 0.2f));
        GUI::Panel(Renderer::screenWidth / 3.0f, Renderer::screenHeight * 0.15f, 0, lastCombo, Renderer::screenHeight * 0.05f, GetComboColor());
    }
public:
    ScoreTracker() : wordsFinished(0), points(0), combo(1.0f), lastCombo(1.0f), comboDecay(0.001f), health(0), lastHealth(0), endCutscene(false), difficultyScaling(1.0f), startTime(0.0f) {}
    void Awake() override
    {
        textRef = AddComponent<Text>("", Renderer::screenWidth / 2.0f, Renderer::screenHeight / 10.0f);
    }
    void FinishedWord(std::string _word, float _mult = 1.0f)
    {
        wordsFinished++;
        uint wordPoints = (int)glm::pow(_word.length(), 1.3f) * _mult;
        points += wordPoints * glm::sqrt(glm::floor(combo));
        health = glm::clamp(health + (wordPoints / 100.0f) * glm::sqrt(combo), 0.0f, 1.0f);
        combo = glm::clamp(combo + wordPoints / 100.0f, 1.0f, 6.999f);
    }
    uint GetWordsFinished()
    {
        return wordsFinished;
    }
    float GetHealth()
    {
        return health;
    }
    void SetEndCutscene(bool _end)
    {
        endCutscene = _end;
    }
};

#endif