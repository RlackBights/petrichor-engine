#include "ptc_console.h"
#include <ptc_component.h>

class ExampleStandaloneComponent : public Component
{
    void Start() override
    {
        Console::WriteLine("Example of a standalone, single-source component");
    }
};