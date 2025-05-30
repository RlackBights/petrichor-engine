#include "ptc_console.hpp"
#include <ptc_component.hpp>

class ExampleStandaloneComponent : public Component
{
    void Start() override
    {
        Console::WriteLine("Example of a standalone, single-source component");
    }
};