#ifndef GUI_STUFF
#define GUI_STUFF

#include "ptc_component.hpp"
#include "ptc_debug.hpp"

class GUIStuff : public Component
{
private:
    
public:
    void Update() override
    {
        Debug::Log("Woah hallo");
    }
};

#endif