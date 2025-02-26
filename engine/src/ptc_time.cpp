#include <ptc_time.h>

Uint64 Time::lastFrame;
Uint64 Time::currentFrame;
float Time::deltaTime;
float Time::deltaTimeUnscaled;
float Time::timeScale;
std::vector<Timer> Time::timers;
float Time::fixedTimeAccumulator;
constexpr const float Time::fixedDeltaTime;

void Time::initTime()
{
	lastFrame = 0;
	timeScale = 1.0f;
	currentFrame = SDL_GetTicks();
	deltaTimeUnscaled = (currentFrame - lastFrame) / 1000.0f;
	deltaTime = deltaTimeUnscaled * timeScale;
	fixedTimeAccumulator = 0.0f;
}
void Time::updateTime()
{
	lastFrame = currentFrame;
	currentFrame = SDL_GetTicks();
	deltaTimeUnscaled = (currentFrame - lastFrame) / 1000.0f;
	deltaTime += deltaTimeUnscaled;

	for (std::vector<Timer>::iterator i = timers.begin(); i != timers.end();)
	{
		i->seconds -= (i->unscaled ? deltaTimeUnscaled : deltaTime);
		if (i->seconds <= 0)
		{
			i->callback();
			i = timers.erase(i);
		}
		else i++;
	}
}
void Time::wrapTime()
{
	deltaTime = 0.0f;
}
bool Time::isNextFrameReady(int FPSLimit)
{
	if ((FPSLimit > 0 && 1.0f / FPSLimit < deltaTime) || FPSLimit <= 0)
	{
		deltaTime *= timeScale;
		return true;
	}

	return false;
}
void Time::createTimer(float seconds, std::function<void()> callback, bool unscaled)
{
	timers.push_back(Timer{ seconds, callback, unscaled });
}