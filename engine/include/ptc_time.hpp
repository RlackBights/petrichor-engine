#ifndef PTC_TIME_HPP
#define PTC_TIME_HPP

#include <SDL3/SDL.h>
#include <functional>
#include <vector>

typedef struct Timer {
	float seconds;
	std::function<void()> callback;
	bool unscaled;
} Timer;

class Time {
public:
	static Uint64 lastFrame;
	static Uint64 currentFrame;
	static float deltaTime;
	static float deltaTimeUnscaled;
	static float timeScale;
	static float time;
	static float fixedAccumulator;
	static const float fixedUpdateFrametime;
	static std::vector<Timer> timers;

	static void initTime();
	static void updateTime();
	static void wrapTime();
	static void createTimer(float seconds, std::function<void()> callback, bool unscaled = false);
};

#endif