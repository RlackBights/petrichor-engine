#pragma once

#include "Timer.h"
#include <chrono>
#include <cstdint>
#include <functional>
#include <vector>

namespace PetrichorEngine::Time {
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

	class Time {
	private:
		static inline std::function<uint64_t()> _getTime = nullptr;
		static TimePoint startTime;
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

		static uint64_t GetTime();
		static void SetTimeFunction(std::function<uint64_t()> fn);
		static void InitializeTime();
		static void UpdateTime();
		static void WrapTime();
		static void CreateTimer(float seconds, std::function<void()> callback, bool unscaled = false);
	};
}
