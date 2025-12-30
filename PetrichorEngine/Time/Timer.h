#pragma once

#include <functional>

namespace PetrichorEngine {
	struct Timer {
		float seconds;
		std::function<void()> callback;
		bool unscaled;
	};
}
