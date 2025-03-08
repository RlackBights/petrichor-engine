#include "ptc_camera.h"
#include "ptc_object.h"
#include "ptc_text.h"
#include "standalone/example_standalone.cpp"
#include <glm/fwd.hpp>

Object testObj("testObj"), camera("camera"), textTest("textTest");

void game_main()
{
    camera.AddComponent<Camera>();

    textTest.AddComponent<Text>("", 0, 0, Font::LoadFont("arial.ttf", 48), glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));
    textTest.AddComponent<ExampleStandaloneComponent>();
}