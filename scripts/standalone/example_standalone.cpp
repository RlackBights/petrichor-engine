#include "ptc_console.hpp"
#include "ptc_debug.hpp"
#include "ptc_text.hpp"
#include <ptc_component.hpp>

class ExampleStandaloneComponent : public Component
{
    void Start() override
    {
        Debug::Log("Example of a standalone, single-source component");
    }

    void Update() override
    {
        Debug::Log("Hellooo");
    }
};