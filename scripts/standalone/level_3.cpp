#include "ptc_component.h"
#include "ptc_math.h"
#include "ptc_object.h"
#include "./text_input.cpp"
#include <functional>
#include <string>

class Level3 : public Component
{
private:
    std::function<std::string(bool _)> getWordRef;
    float switchTime = 4.0f;
    float lastSwitchTime = 4.0f;
    Text* textRef;
    void FixedUpdate() override
    {

        glm::vec4 currCol = textRef->GetTextColor();
        currCol.w = Math::Lerp(currCol.w, 1.0f, 0.1f);
        textRef->SetTextColor(currCol);
        switchTime -= Time::deltaTime;
        if (switchTime <= 0) 
        {
            lastSwitchTime *= 0.998f;
            switchTime = lastSwitchTime;
            GetComponent<TextInput>()->RefreshText(getWordRef(true));
        }
    }
public:
    Level3(std::function<std::string(bool _)> _getWordRef)
    {
        getWordRef = _getWordRef;
    }
    void Awake() override
    {
        textRef = GetComponent<Text>();
    }
};