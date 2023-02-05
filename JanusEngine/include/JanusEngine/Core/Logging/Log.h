#pragma once
#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "Core/Logging/LogSink.h"
#include "Core/Logging/LogGroup.h"

namespace JanusEngine
{
    class Log
    {
    public:
        Log();
        virtual ~Log();

        static spdlog::logger& getLoggerFromLogGroup(LogGroup logGroup);
        static inline Log& getInstance() {if(instance != nullptr) return *instance; instance = new Log(); return *instance;}

        void registerLogGroup(const std::string &loggerName);
        void deregisterLogGroup(const std::string &loggerName);    
        void registerLogSink(LogSink sink);

    private:
        static Log* instance;
        std::unordered_map<std::string, spdlog::logger> logGroups;
        std::vector<LogSink> logSinks;
    };
}

#define JN_TRACE(logGroup, ...)         Log::getLoggerFromLogGroup()->trace(__VA_ARGS__)
#define JN_DEBUG(logGroup, ...)         Log::getLoggerFromLogGroup()->debug(__VA_ARGS__)
#define JN_INFO(logGroup, ...)          Log::getLoggerFromLogGroup()->info(__VA_ARGS__)
#define JN_WARN(logGroup, ...)          Log::getLoggerFromLogGroup()->warn(__VA_ARGS__)
#define JN_ERROR(logGroup, ...)         Log::getLoggerFromLogGroup()->error(__VA_ARGS__)
#define JN_CRITICAL(logGroup, ...)      Log::getLoggerFromLogGroup()->critical(__VA_ARGS__)
