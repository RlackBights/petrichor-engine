#include "Logger/CustomLogger.h"
#include <Core/Log.h>

using namespace PetrichorEngine;

namespace PetrichorEditor {
    int main()
    {
        Log::Info("Test info default");
        Log::Warn("Test warning default");
        Log::Error("Test error default");

        CustomLogger logger = CustomLogger();
        Log::SetLogger(&logger);

        Log::Info("Custom info");
        Log::Warn("Custom warning");
        Log::Error("Custom error");

        return 0;
    }
}

int main()
{
    return PetrichorEditor::main();
}

