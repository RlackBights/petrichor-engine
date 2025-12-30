#pragma once

#include "Timer.h"
#include <functional>
#include <vector>

namespace PetrichorEngine {
	class Time {
	public:
		static unsigned long long lastFrame;
		static unsigned long long currentFrame;
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
}
