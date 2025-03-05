#include "ptc_object.h"
#include "standalone/example_standalone.cpp"
#include "example_header.h"

Object testObj("testObj");

void game_main()
{
    testObj.AddComponent<ExampleStandaloneComponent>();
    testObj.AddComponent<ExampleComponent>();
}