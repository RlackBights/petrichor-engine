#include "ptc_camera.h"
#include "ptc_object.h"
#include "ptc_text.h"
#include "standalone/welcome_text_manager.cpp"
#include "standalone/text_input.cpp"
#include <glm/fwd.hpp>

Object camera("camera"), textTest("textTest"), textTest2("textTest2");

void game_main()
{
    camera.AddComponent<Camera>();

    textTest.AddComponent<Text>("", 0, 0, Font::LoadFont("chiller.ttf", 48), glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));
    textTest.AddComponent<WelcomeTextManager>();

    textTest2.AddComponent<Text>("", 0, 0, Font::LoadFont("chiller.ttf", 48));
    textTest2.AddComponent<TextInput>();
}