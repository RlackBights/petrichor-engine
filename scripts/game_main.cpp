#include "ptc_camera.h"
#include "ptc_object.h"
#include "ptc_text.h"
#include "standalone/welcome_text_manager.cpp"
#include <glm/fwd.hpp>

Object camera("camera"), textTest("textTest");

void game_main()
{
    camera.AddComponent<Camera>();

    textTest.AddComponent<Text>("", 0, 0, Font::LoadFont("chiller.ttf", 48), glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));
    textTest.AddComponent<WelcomeTextManager>();
}