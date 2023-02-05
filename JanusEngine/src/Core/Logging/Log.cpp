
#include "Core/Logging/Log.h"

namespace JanusEngine
{
    Log::Log()
    {
        assert(instance == nullptr);
        instance = this;
    }

    Log::~Log()
    {
    }

    spdlog::logger *Log::getLoggerFromLogGroup(LogGroup logGroup)
    {
        return nullptr;
    }

    void Log::registerLogGroup(const std::string &loggerName)
    {
    }

    void Log::deregisterLogGroup(const std::string &loggerName)
    {
    }

    void Log::registerLogSink(LogSink sink)
    {
    }
}