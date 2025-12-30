#include "Logger/CustomLogger.h"
#include <iostream>

namespace PetrichorEditor {
    void CustomLogger::Info(const std::string& message) { std::cout << "INFOOOO__ " << message << "\r\n"; }
    void CustomLogger::Warn(const std::string& message) { std::cout << "WARNNN__ " << message << "\r\n"; }
    void CustomLogger::Error(const std::string& message) { std::cout << "ERRRORRRRRR__ " << message << "\r\n"; }
}