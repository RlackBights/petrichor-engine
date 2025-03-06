#include "ptc_console.h"
#include "ptc_renderer.h"
#include "ptc_text.h"
#include <ptc_component.h>

class ExampleStandaloneComponent : public Component
{
    void Start() override
    {
        Console::WriteLine("Example of a standalone, single-source component");
    }
    void Update() override
    {
        Text::renderText("HELLOOOO", 0.0f, 0.0f, 1.0f, Renderer::screenWidth, Renderer::screenHeight);
    }
};