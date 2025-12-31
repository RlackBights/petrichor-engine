#include "Core/ConsoleLogger.h"
#include <iostream>

namespace PetrichorEngine::Core {
    void ConsoleLogger::Info(const std::string& message) { std::cout << "[INFO] " << message << "\r\n"; }
    void ConsoleLogger::Warn(const std::string& message) { std::cout << "[WARNING] " << message << "\r\n"; }
    void ConsoleLogger::Error(const std::string& message) { std::cout << "[ERROR] " << message << "\r\n"; }
}