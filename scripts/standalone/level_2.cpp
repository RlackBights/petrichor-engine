#include "ptc_component.h"
#include "ptc_math.h"
#include "ptc_renderer.h"
#include "ptc_text.h"
#include "ptc_time.h"
#include <cstdlib>
class Level2 : public Component
{
private:
    Text* textRef;
    float targetX, targetY, currX, currY;
    void FixedUpdate() override
    {
        currX = Math::Lerp(currX, targetX, 0.1f);
        currY = Math::Lerp(currY, targetY, 0.1f);
        textRef->MoveText(currX, currY);
    }
    void SetRandomPlace()
    {
        int rnd = rand();
        if (rnd >= RAND_MAX / 2) {
            rnd -= RAND_MAX / 2;
            float prc = rnd / (RAND_MAX / 2.0f);
            currX = Renderer::screenWidth + textRef->getPixelWidth() / 2.0f;
            currY = Renderer::screenHeight * (0.75f * prc + 0.125f);
            targetX = currX;
            Time::createTimer(0.5f, [&]() { targetX = currX - textRef->getPixelWidth() * 1.2f; });
            targetY = currY;
            textRef->MoveText(currX, currY);
        } else {
            float prc = rnd / (RAND_MAX / 2.0f);
            currX = -textRef->getPixelWidth() / 2.0f;
            currY = Renderer::screenHeight * (0.75f * prc + 0.125f);
            targetX = currX;
            Time::createTimer(0.5f, [&]() { targetX = currX + textRef->getPixelWidth() * 1.2f; });
            targetY = currY;
            textRef->MoveText(currX, currY);
        }
    }
public:
    void Awake() override
    {
        textRef = GetComponent<Text>();
        SetRandomPlace();
    }
    void StartMoveAnimation()
    {
        targetX += (currX < Renderer::screenWidth / 2.0f) ? -textRef->getPixelWidth() * 2.0f : textRef->getPixelWidth() * 2.0f;
        Time::createTimer(1.0f, [&]() { SetRandomPlace(); });
    }
};