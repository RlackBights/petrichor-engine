#include "ptc_camera.h"
#include "ptc_object.h"
#include "standalone/example_standalone.cpp"
#include "example_header.h"

Object testObj("testObj"), camera("camera");

void game_main()
{
    camera.AddComponent<Camera>();

    testObj.AddComponent<ExampleStandaloneComponent>();
    testObj.AddComponent<ExampleComponent>();
}