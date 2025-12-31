#pragma once

#include <functional>

namespace PetrichorEngine::Time {
	struct Timer {
		float seconds;
		std::function<void()> callback;
		bool unscaled;
	};
}
