#ifndef PTC_DEBUG_HPP
#define PTC_DEBUG_HPP

#include "ptc_console.hpp"
#include "ptc_debug_structs.hpp"
#include <string>

class Debug
{
private:
    static bool groupLogs;
    static std::vector<std::pair<LogType, std::string>> logs;
public:
    static void SwitchLogGrouping();
    static void SetLogGroupimg(bool groupLogs);
    static const bool GetLogGrouping();
    template <class T>
    static void Log(T message)
    {
        logs.push_back({LogType::LOG_INFO, Console::ToString(message)});
    }
    template <class T>
    static void Warn(T message)
    {
        logs.push_back({LogType::LOG_WARNING, Console::ToString(message)});
    }
    template <class T>
    static void Error(T message)
    {
        logs.push_back({LogType::LOG_ERROR, Console::ToString(message)});
    }

    static void Clear();
    static std::vector<std::pair<LogType, std::string>>& GetLogs();
};

#endif