#pragma once

#include <string>

namespace PetrichorEngine
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;
        virtual void Info(const std::string& message) = 0;
        virtual void Warn(const std::string& message) = 0;
        virtual void Error(const std::string& message) = 0;
    };
}