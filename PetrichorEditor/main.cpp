#include "Logger/CustomLogger.h"
#include "Time/Time.h"
#include <Core/Log.h>
#include <unistd.h>

using namespace PetrichorEngine;
using namespace PetrichorEngine::Core;

namespace PetrichorEditor {
    int main()
    {
        Log::Info(Time::Time::GetTime());

        CustomLogger logger = CustomLogger();
        Log::SetLogger(&logger);
        usleep(1000000);

        Log::Info(Time::Time::GetTime());

        return 0;
    }
}

int main()
{
    return PetrichorEditor::main();
}

