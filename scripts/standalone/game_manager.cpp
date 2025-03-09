#include "ptc_component.h"
#include "ptc_console.h"
#include "ptc_object.h"
#include "./welcome_text_manager.cpp"
#include "./text_input.cpp"
#include "ptc_renderer.h"
#include "glm/fwd.hpp"
#include <fstream>

#define TEXT_PATH "resources/text_files/"

Object welcomeText("welcomeText"), textLevel1("textLevel1");

class GameManager : public Component
{
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
        welcomeText.AddComponent<WelcomeTextManager>()->Start();
        textLevel1.AddComponent<TextInput>(getRandomWord(), Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), false, false,
        [&](TextInput* _self) {
            _self->enabled = false;
            _self->RefreshText(getRandomWord());
            _self->enabled = true;
        })->Start();
    }
};