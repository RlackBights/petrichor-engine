#include "ptc_camera.h"
#include "ptc_object.h"
#include "ptc_text.h"
#include "standalone/example_standalone.cpp"
#include "example_header.h"

Object testObj("testObj"), camera("camera"), textTest("textTest");

void game_main()
{
    camera.AddComponent<Camera>();

    testObj.AddComponent<ExampleComponent>();

    textTest.AddComponent<Text>("HELLOOOOOOO");
}