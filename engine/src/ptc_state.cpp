#include "ptc_state.h"

void State::SetArguments(int _argc, char** _argv)
{
    argumentCount = _argc;
    arguments = _argv;
}

int State::argumentCount = 1;
char** State::arguments = nullptr;