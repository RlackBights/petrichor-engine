#include "ptc_component.h"
#include "ptc_console.h"
#include "ptc_object.h"
#include "./welcome_text_manager.cpp"
#include "./score_tracker.cpp"
#include "./text_input.cpp"
#include "ptc_renderer.h"
#include "glm/fwd.hpp"
#include <fstream>
#include <string>
#include <vector>

#define TEXT_PATH "resources/text_files/"

Object welcomeText("welcomeText"), scoreTracker("scoreTracker"), textLevel1("textLevel1");

class GameManager : public Component
{
private:
    ScoreTracker* scoreRef;
    WelcomeTextManager* cutsceneRef;
    std::vector<TextInput*> textLevels;
public:
    std::string getRandomWord(const std::string& filename = std::string(TEXT_PATH) + "2of12inf_filtered.txt")
    {
        std::ifstream file(filename);
        std::string word, chosenWord;
        int count = 0;
    
        if (!file) {
            Console::WriteLine("Error opening file");
            return "";
        }
    
        while (std::getline(file, word)) {
            if (rand() % (++count) == 0) { // Reservoir sampling technique
                chosenWord = word;
            }
        }
        chosenWord[0] -= 32;
        return chosenWord;
    }
    void Start() override
    {
        cutsceneRef = welcomeText.AddComponent<WelcomeTextManager>();
        scoreRef = scoreTracker.AddComponent<ScoreTracker>();
        textLevels.push_back(textLevel1.AddComponent<TextInput>(getRandomWord(), Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), false, false));
        textLevels[0]->SetCompleteFunction([&](TextInput* _self) {
            _self->enabled = false;
            scoreRef->FinishedWord(textLevels[0]->GetText());
            _self->RefreshText(getRandomWord());
            _self->enabled = true;
            if (scoreRef->GetWordsFinished() == 3)
            {
                _self->DisableWriting(true);
                // glm::vec4 col = _self->GetBaseColor();
                // _self->SetBaseColor();
                Text* textRef = cutsceneRef->GetTextRef();
                textRef->MoveText(Renderer::screenWidth / 2.0f, (Renderer::screenHeight + textRef->GetFont()->fontSize * 4) / 2.0f);
                cutsceneRef->GetTextRef()->SetText("Alright, check this out...");
                cutsceneRef->AdvanceCutscene();
                Time::createTimer(3.0f, [&]() { textLevels[0]->DisableWriting(false); });
            }
        });
    }
};