#pragma once

#include "Debug/DebugStructs.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

namespace PetrichorEngine {
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
            oss << value;
            logs.push_back({LogType::LOG_INFO, oss.str()});
        }
        template <typename T>
        static void Warn(const T& value)
        {
            std::ostringstream oss;
            oss << value;
            logs.push_back({LogType::LOG_WARNING, oss.str()});
        }
        template <typename T>
        static void Error(const T& value)
        {
            std::ostringstream oss;
            oss << value;
            logs.push_back({LogType::LOG_ERROR, oss.str()});
        }

        static void Clear();
        static std::vector<std::pair<LogType, std::string>>& GetLogs();
    };
}
