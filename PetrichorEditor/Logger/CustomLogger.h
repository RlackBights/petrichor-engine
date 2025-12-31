#pragma once

#include "Core/ILogger.h"

using namespace PetrichorEngine;

namespace PetrichorEditor {
    class CustomLogger : public Core::ILogger
    {
        public:
            void Info(const std::string& message) override;
            void Warn(const std::string& message) override;
            void Error(const std::string& message) override;
    };
}
