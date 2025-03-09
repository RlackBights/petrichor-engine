#include "ptc_component.h"
#include "ptc_object.h"
#include "./welcome_text_manager.cpp"
#include "./text_input.cpp"
#include "ptc_renderer.h"
#include "glm/fwd.hpp"

Object welcomeText("welcomeText"), textLevel1("textLevel1");

class GameManager : public Component
{
    void Start() override
    {
        welcomeText.AddComponent<WelcomeTextManager>()->Start();
        textLevel1.AddComponent<TextInput>("HelloThere", Renderer::screenWidth / 2.0f, Renderer::screenHeight / 2.0f, glm::vec4(0.2f, 0.8f, 0.8f, 1.0f), false, false,
        [](TextInput* _self) {
            _self->enabled = false;
            _self->RefreshText("NewText");
            _self->enabled = true;
        })->Start();
    }
};