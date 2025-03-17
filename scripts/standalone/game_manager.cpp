#include "ptc_component.h"
#include "ptc_console.h"
#include "ptc_object.h"
#include "./welcome_text_manager.cpp"
#include "./score_tracker.cpp"
#include "./text_input.cpp"
#include "./level_2.cpp"
#include "./level_3.cpp"
#include "./level_4.cpp"
#include "ptc_renderer.h"
#include "glm/fwd.hpp"
#include "ptc_time.h"
#include <fstream>
#include <string>
#include <vector>

#define TEXT_PATH "resources/text_files/"

Object welcomeText("welcomeText"), scoreTracker("scoreTracker"), textLevel1("textLevel1"), textLevel2("textLevel2"), textLevel3("textLevel3"), textLevel4("textLevel4");

class GameManager : public Component
{
private:
    ScoreTracker* scoreRef;
    WelcomeTextManager* cutsceneRef;
    std::vector<TextInput*> textLevels;
    std::vector<std::string> words;
    bool skipIntro;
    bool endGame;

    void FixedUpdate() override
    {
        if (!endGame && scoreRef->GetEndCutscene() && scoreRef->GetHealth() <= 0.0001f)
        {
            endGame = true;
            for (TextInput* in : textLevels)
            {
                in->DisableWriting(true);
                in->enabled = false;
            }
            Time::createTimer(3.0f, []() { exit(0); });
        }
    }
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
    std::string GetRandomWord(bool _letter = false)
    {
        return _letter ? Console::FormatString("%c", (words[rand() % words.size()][0] - 'A' + 'a')) : words[rand() % words.size()];
    }
    GameManager(bool _skipIntro = false) : skipIntro(_skipIntro) {}
    void Start() override
    {
        endGame = false;
        LoadWords();

        scoreRef = scoreTracker.AddComponent<ScoreTracker>();
        if (!skipIntro) cutsceneRef = welcomeText.AddComponent<WelcomeTextManager>();
        textLevels.push_back(textLevel1.AddComponent<TextInput>(GetRandomWord(), Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), false, false));
        textLevels.push_back(textLevel2.AddComponent<TextInput>(GetRandomWord(), Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), true, false));
        textLevels.push_back(textLevel3.AddComponent<TextInput>(GetRandomWord(true), Renderer::screenWidth / 3.0f - 48.0f, Renderer::screenHeight * 0.15f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), true, false));
        textLevels.push_back(textLevel4.AddComponent<TextInput>(GetRandomWord(), Renderer::screenWidth / 2.0f, Renderer::screenHeight / 3.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), true, false));
        textLevels[0]->SetCompleteFunction([&](TextInput* _self) {
            scoreRef->FinishedWord(textLevels[0]->GetText());
            _self->RefreshText(GetRandomWord());
            if (!skipIntro)
            {
                if (!scoreRef->GetEndCutscene() && scoreRef->GetWordsFinished() == 3)
                {
                    cutsceneRef->AdvanceCutscene();
                    cutsceneRef->GetTextRef()->SetText("Alright, check this out...");
                    Time::createTimer(3.0f, [&]() { cutsceneRef->GetTextRef()->SetText("This is your health\nYou can restore it by typing"); });
                    Time::createTimer(8.0f, [&]() { cutsceneRef->GetTextRef()->SetText("It will decrease over time\nIf it runs out, you die"); });
                    Time::createTimer(13.0f, [&]() { cutsceneRef->GetTextRef()->SetText("All good? Let's get into it"); });
                    Time::createTimer(16.0f, [&]() { cutsceneRef->SetEnd(true); scoreRef->SetEndCutscene(true); });
                    Time::createTimer(26.0f, [&]() { textLevel2.enabled = true; });
                    Time::createTimer(36.0f, [&]() { textLevel3.enabled = true; });
                    Time::createTimer(46.0f, [&]() { textLevel4.enabled = true; });
                }
            }
        });
        if (skipIntro)
        {
            scoreRef->SetEndCutscene(true);
            Time::createTimer(10.0f, [&]() { textLevel2.enabled = true; });
            Time::createTimer(20.0f, [&]() { textLevel3.enabled = true; });
            Time::createTimer(30.0f, [&]() { textLevel4.enabled = true; });
        } else
        {
            textLevels[0]->GetComponent<Text>()->enabled = false;
            textLevels[0]->enabled = false;
            Time::createTimer(6.0f, [&]() { textLevels[0]->GetComponent<Text>()->enabled = true; textLevels[0]->enabled = true; });
        }

        textLevels[1]->AddComponent<Level2>();
        textLevels[1]->SetCompleteFunction([&](TextInput* _self) {
            _self->enabled = false;
            scoreRef->FinishedWord(textLevels[1]->GetText(), 1.3f);
            Time::createTimer(0.9f, [&]() { textLevels[1]->RefreshText(GetRandomWord()); textLevels[1]->enabled = true; });
            _self->GetComponent<Level2>()->StartMoveAnimation();
        });
        textLevel2.enabled = false;

        textLevels[2]->AddComponent<Level3>([&](bool _) -> std::string { return GetRandomWord(_); });
        textLevels[2]->SetCompleteFunction([&](TextInput* _self) {
            _self->enabled = false;
            scoreRef->FinishedWord(textLevels[2]->GetText(), 0.0f, 0.025f);
            _self->RefreshText(_self->GetText());
            _self->enabled = true;
        });
        textLevel3.enabled = false;

        textLevels[3]->AddComponent<Level4>();
        textLevels[3]->SetCompleteFunction([&](TextInput* _self) {
            _self->enabled = false;
            scoreRef->FinishedWord(textLevels[3]->GetText(), 1.5f);
            _self->RefreshText(GetRandomWord());
            _self->enabled = true;
        });
        textLevel4.enabled = false;
    }
};