#include "ptc_component.h"
#include "ptc_console.h"
#include "ptc_text.h"
#include <glm/common.hpp>
#include <glm/trigonometric.hpp>

class ScoreTracker : public Component
{
private:
    uint wordsFinished;
    uint points;
    uint comboLevel;
    float health;
    Text* textRef;

    void FixedUpdate() override
    {
        textRef->SetText(Console::FormatString("Words finished: %d\nPoints: %d", wordsFinished, points));
    }
public:
    ScoreTracker() : wordsFinished(0), points(0), comboLevel(0), health(0) {}
    void Awake() override
    {
        textRef = AddComponent<Text>("Hellooo", Renderer::screenWidth / 2.0f, Renderer::screenHeight / 10.0f);
        textRef->SetTextAnimation([&](float _x) {
            return glm::sin(_x + points) * comboLevel;
        });
    }
    void FinishedWord(std::string _word)
    {
        wordsFinished++;
        points += (int)glm::pow(_word.length(), 1.3f);
    }
    uint GetWordsFinished()
    {
        return wordsFinished;
    }
};