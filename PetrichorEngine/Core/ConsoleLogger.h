#pragma once

#include "Core/ILogger.h"
#include <string>

namespace PetrichorEngine
{
    class ConsoleLogger : public ILogger
    {
    public:
        ConsoleLogger() = default;
        ~ConsoleLogger() = default;
        
        void Info(const std::string& message) override;
        void Warn(const std::string& message) override;
        void Error(const std::string& message) override;
    };
}