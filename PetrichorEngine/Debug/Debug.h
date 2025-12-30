#pragma once

#include "Debug/DebugStructs.h"
#include <sstream>
#include <string>
#include <vector>

class Debug
{
private:
    static bool groupLogs;
    static std::vector<std::pair<LogType, std::string>> logs;
public:
    static void SwitchLogGrouping();
    static void SetLogGroupimg(bool groupLogs);
    static const bool GetLogGrouping();
    template <typename T>
    static void Log(const T& value)
    {
        std::ostringstream oss;
        logs.push_back({LogType::LOG_INFO, (oss << value).str()});
    }
    template <typename T>
    static void Warn(const T& value)
    {
        std::ostringstream oss;
        logs.push_back({LogType::LOG_WARNING, (oss << value).str()});
    }
    template <typename T>
    static void Error(const T& value)
    {
        std::ostringstream oss;
        logs.push_back({LogType::LOG_ERROR, (oss << value).str()});
    }

    static void Clear();
    static std::vector<std::pair<LogType, std::string>>& GetLogs();
};