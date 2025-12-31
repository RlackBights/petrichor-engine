#include "Time.h"
#include "Core/Log.h"
#include <cstdint>
#include <string>

namespace PetrichorEngine::Time {
	void Time::SetTimeFunction(std::function<uint64_t ()> fn)
	{
		_getTime = fn;
	}
	uint64_t Time::GetTime()
	{
		if (!_getTime) _getTime = []()->uint64_t { return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - startTime).count(); };
		return _getTime();
	}
	void Time::InitializeTime()
	{
		lastFrame = 0;
		timeScale = 1.0f;
		currentFrame = GetTime();
		time = 0.0f;
		deltaTimeUnscaled = (currentFrame - lastFrame) / 1000.0f;
		deltaTime = deltaTimeUnscaled * timeScale;
	}
	void Time::UpdateTime()
	{
		lastFrame = currentFrame;
		currentFrame = GetTime();
		deltaTimeUnscaled = (currentFrame - lastFrame) / 1000.0f;
		deltaTime = deltaTimeUnscaled * timeScale;
		fixedAccumulator += deltaTimeUnscaled;
	}
	void Time::WrapTime()
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
						Core::Log::Error("Timer error: " + std::string(e.what()));
					}
				} else
				{
					Core::Log::Error("Timer callback function not found");
				}

				i = timers.erase(i);
			}
			else i++;
		}
		time += deltaTime;
	}
	void Time::CreateTimer(float seconds, std::function<void()> callback, bool unscaled)
	{
		if (!callback)
		{
			Core::Log::Error("Received empty callback!");
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
	TimePoint Time::startTime = Clock::now();
}

