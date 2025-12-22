#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "ptc_console.hpp"
#include "ptc_debug.hpp"
#include "ptc_text.hpp"
#include "ptc_time.hpp"
#include <ptc_component.hpp>

class ExampleStandaloneComponent : public Component
{
    void Start() override
    {
        Debug::Log("Example of a standalone, single-source component");
    }

    void Update() override
    {
        transform->position =  {glm::sin(Time::time) * 3, 2, glm::cos(Time::time) * 3 };
    }
};