#include "ptc_debug.hpp"
#include "ptc_debug_structs.hpp"
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
void Debug::Log(std::string message)
{
    logs.push_back({LogType::INFO, message});
}

void Debug::Warn(std::string message)
{
    logs.push_back({LogType::WARNING, message});
}

void Debug::Error(std::string message)
{
    logs.push_back({LogType::ERROR, message});
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