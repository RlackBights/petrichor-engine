#include "Core/Log.h"
#include "Core/ILogger.h"

namespace PetrichorEngine::Core {
    void Log::SetLogger(ILogger* _logger) { Log::_logger = _logger; }
    ILogger* Log::GetLogger() { return (!_logger ? &_defaultLogger : _logger); }
}