#include "Time.h"
#include "Core/Log.h"
#include <string>

namespace PetrichorEngine {
	void Time::initTime()
	{
		lastFrame = 0;
		timeScale = 1.0f;

		Log::Error("CURRENT TIME IS UNIMPLEMENTED!!!!");
		currentFrame = 0; //SDL_GetTicks();
		time = 0.0f;
		deltaTimeUnscaled = (currentFrame - lastFrame) / 1000.0f;
		deltaTime = deltaTimeUnscaled * timeScale;
	}
	void Time::updateTime()
	{
		lastFrame = currentFrame;

		Log::Error("CURRENT TIME IS UNIMPLEMENTED!!!!");
		currentFrame = 0; //SDL_GetTicks();
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
						Log::Error("Timer error: " + std::string(e.what()));
					}
				} else
				{
					Log::Error("Timer callback function not found");
				}

				i = timers.erase(i);
			}
			else i++;
		}
		time += deltaTime;
	}
	void Time::createTimer(float seconds, std::function<void()> callback, bool unscaled)
	{
		if (!callback)
		{
			Log::Error("Received empty callback!");
			return;
		}
		timers.push_back(Timer{ seconds, std::move(callback), unscaled });
	}

	unsigned long long Time::lastFrame;
	unsigned long long Time::currentFrame;
	float Time::deltaTime;
	float Time::deltaTimeUnscaled;
	float Time::timeScale;
	float Time::time;
	float Time::fixedAccumulator;
	const float Time::fixedUpdateFrametime = 0.02f; // 50 frames a second intended, similar to Unity
	std::vector<Timer> Time::timers;
}

