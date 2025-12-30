#pragma once

#include "Core/ConsoleLogger.h"
#include "Core/ILogger.h"
#include <sstream>

namespace PetrichorEngine
{
    class Log
    {
    private:
        static inline ILogger* _logger;
        static inline ConsoleLogger _defaultLogger = ConsoleLogger();
    public:
        static void SetLogger(ILogger* _logger);

        static ILogger* GetLogger();

        template <typename T>
        static void Info(const T& value)
        {
            std::ostringstream oss;
            oss << value;
            GetLogger()->Info(oss.str());
        }
        
        template <typename T>
        static void Warn(const T& value)
        {
            std::ostringstream oss;
            oss << value;
            GetLogger()->Warn(oss.str());
        }

        template <typename T>
        static void Error(const T& value)
        {
            std::ostringstream oss;
            oss << value;
            GetLogger()->Error(oss.str());
        }
    };
}