#include "Debug/Debug.h"
#include "Debug/DebugStructs.h"
#include <string>
#include <utility>
#include <vector>

void Debug::SwitchLogGrouping()
{
    Debug::groupLogs = !Debug::groupLogs;
}
void Debug::SetLogGroupimg(bool groupLogs)
{
    Debug::groupLogs = groupLogs;
}
const bool Debug::GetLogGrouping()
{
    return Debug::groupLogs;
}

void Debug::Clear()
{
    logs.clear();
}

std::vector<std::pair<LogType, std::string>>& Debug::GetLogs()
{
    return logs;
}

bool Debug::groupLogs = true;
std::vector<std::pair<LogType, std::string>> Debug::logs; 