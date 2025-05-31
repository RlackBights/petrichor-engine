#include "ptc_console.hpp"
#include "ptc_text.hpp"
#include <ptc_component.hpp>

class ExampleStandaloneComponent : public Component
{
    Text* textRef;
    void Start() override
    {
        textRef = GetComponent<Text>();
        Console::WriteLine("Example of a standalone, single-source component");
    }

    void Update() override
    {
        textRef->SetText(Console::FormatString("%f.2 %f.2 %f.2", parentObject->transform.position.x, parentObject->transform.position.y, parentObject->transform.position.z));
    }
};