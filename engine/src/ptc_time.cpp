#include "ptc_console.hpp"
#include <exception>
#include <ptc_time.hpp>

Uint64 Time::lastFrame;
Uint64 Time::currentFrame;
float Time::deltaTime;
float Time::deltaTimeUnscaled;
float Time::timeScale;
float Time::time;
float Time::fixedAccumulator;
const float Time::fixedUpdateFrametime = 0.02f; // 50 frames a second intended, similar to Unity
std::vector<Timer> Time::timers;

void Time::initTime()
{
	lastFrame = 0;
	timeScale = 1.0f;
	currentFrame = SDL_GetTicks();
	time = 0.0f;
	deltaTimeUnscaled = (currentFrame - lastFrame) / 1000.0f;
	deltaTime = deltaTimeUnscaled * timeScale;
}
void Time::updateTime()
{
	lastFrame = currentFrame;
	currentFrame = SDL_GetTicks();
	deltaTimeUnscaled = (currentFrame - lastFrame) / 1000.0f;
	deltaTime = deltaTimeUnscaled * timeScale;
	fixedAccumulator += deltaTimeUnscaled;
}
void Time::wrapTime()
{
	for (std::vector<Timer>::iterator i = timers.begin(); i != timers.end();)
	{
		i->seconds -= (i->unscaled ? deltaTimeUnscaled : deltaTime);
		if (i->seconds <= 0)
		{
			if (i->callback)
			{
				try
				{
					i->callback();
				} catch (const std::exception e) 
				{
					Console::WriteLine(Console::FormatString("Timer error: %s", e.what()), Color::RED, false);
				}
			} else
			{
				Console::WriteLine("ERROR: Timer callback function not found", Color::RED, false);
			}

			i = timers.erase(i);
		}
		else i++;
	}
	time += deltaTime;
	deltaTime = 0.0f;
	deltaTimeUnscaled = 0.0f;
}
void Time::createTimer(float seconds, std::function<void()> callback, bool unscaled)
{

    if (!callback)
    {
        Console::WriteLine("ERROR: Received empty callback!");
        return;
    }
	timers.push_back(Timer{ seconds, std::move(callback), unscaled });
}