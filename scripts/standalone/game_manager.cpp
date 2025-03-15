#include "ptc_component.h"
#include "ptc_console.h"
#include "ptc_object.h"
#include "./welcome_text_manager.cpp"
#include "./score_tracker.cpp"
#include "./text_input.cpp"
#include "./level_2.cpp"
#include "ptc_renderer.h"
#include "glm/fwd.hpp"
#include <fstream>
#include <string>
#include <vector>

#define TEXT_PATH "resources/text_files/"

Object welcomeText("welcomeText"), scoreTracker("scoreTracker"), textLevel1("textLevel1"), textLevel2("textLevel2");

class GameManager : public Component
{
private:
    ScoreTracker* scoreRef;
    WelcomeTextManager* cutsceneRef;
    std::vector<TextInput*> textLevels;
    std::vector<std::string> words;
public:
    // preload words from list
    void LoadWords(const std::string& filename = std::string(TEXT_PATH) + "2of12inf_filtered.txt")
    {
        std::ifstream file(filename);
        std::string word;
        int count = 0;
    
        if (!file) {
            Console::WriteLine("Error opening file");
            return;
        }
    
        while (std::getline(file, word)) {

            word[0] -= 32;
            words.push_back(word);
        }
    }
    std::string GetRandomWord()
    {
        return words[rand() % words.size()];
    }
    void Start() override
    {
        LoadWords();

        scoreRef = scoreTracker.AddComponent<ScoreTracker>();
        cutsceneRef = welcomeText.AddComponent<WelcomeTextManager>();
        textLevels.push_back(textLevel1.AddComponent<TextInput>(GetRandomWord(), Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), false, false));
        textLevels.push_back(textLevel2.AddComponent<TextInput>(GetRandomWord(), Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), true, false));
        textLevels[0]->SetCompleteFunction([&](TextInput* _self) {
            scoreRef->FinishedWord(textLevels[0]->GetText());
            _self->RefreshText(GetRandomWord());
            if (scoreRef->GetWordsFinished() == 3)
            {
                cutsceneRef->AdvanceCutscene();
                cutsceneRef->GetTextRef()->SetText("Alright, check this out...");
                Time::createTimer(3.0f, [&]() { cutsceneRef->GetTextRef()->SetText("This is your health\nYou can restore it by typing"); });
                Time::createTimer(8.0f, [&]() { cutsceneRef->GetTextRef()->SetText("It will decrease over time\nIf it runs out, you die"); });
                Time::createTimer(13.0f, [&]() { cutsceneRef->GetTextRef()->SetText("All good? Let's get into it"); });
                Time::createTimer(16.0f, [&]() { cutsceneRef->SetEnd(true); scoreRef->SetEndCutscene(true); });
                Time::createTimer(26.0f, [&]() { textLevel2.enabled = true; });
            }
        });

        textLevels[1]->AddComponent<Level2>();
        textLevels[1]->SetCompleteFunction([&](TextInput* _self) {
            _self->enabled = false;
            scoreRef->FinishedWord(textLevels[1]->GetText(), 1.3f);
            Time::createTimer(0.9f, [&]() { textLevels[1]->RefreshText(GetRandomWord()); textLevels[1]->enabled = true; });
            _self->GetComponent<Level2>()->StartMoveAnimation();
        });
        textLevel2.enabled = false;
    }
};