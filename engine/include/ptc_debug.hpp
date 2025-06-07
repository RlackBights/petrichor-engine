#ifndef PTC_DEBUG_HPP
#define PTC_DEBUG_HPP

#include "ptc_component.hpp"
#include "ptc_debug_structs.hpp"
#include <string>

class Debug : public Component
{
private:
    static bool groupLogs;
    static std::vector<std::pair<LogType, std::string>> logs;
public:
    static void SwitchLogGrouping();
    static void SetLogGroupimg(bool groupLogs);
    static const bool GetLogGrouping();
    static void Log(std::string message);
    static void Warn(std::string message);
    static void Error(std::string message);
    static void Clear();
    static std::vector<std::pair<LogType, std::string>>& GetLogs();
};

#endif