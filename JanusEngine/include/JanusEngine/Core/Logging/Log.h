#pragma once
#include "Core/Core.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"
#include "Core/Logging/LogSink.h"

namespace JanusEngine
{

    class Log
    {
    public:
        struct LogGroupProps
        {
            std::string name;
            std::string formatString;
        };

        static inline std::shared_ptr<spdlog::logger> getLoggerFromLogGroupName(const std::string &logGroupName)
        {
            return spdlog::get(logGroupName);
        }

        template <typename... Args>
        static inline void dispatchLog(const std::string &logGroupName, spdlog::level::level_enum level, Args... args)
        {
            std::shared_ptr<spdlog::logger> logger = getLoggerFromLogGroupName(logGroupName);
            if (logger == nullptr)
                return;
            switch (level)
            {
            case spdlog::level::trace:
                logger->trace(args...);
                break;
            case spdlog::level::debug:
                logger->debug(args...);
                break;
            case spdlog::level::info:
                logger->info(args...);
                break;
            case spdlog::level::warn:
                logger->warn(args...);
                break;
            case spdlog::level::err:
                logger->error(args...);
                break;
            case spdlog::level::critical:
                logger->critical(args...);
                break;
            default:
                logger->info(args...);
            }
        }

        static void registerLogGroup(LogGroupProps logGroupProps);

        static inline void deregisterLogGroup(const std::string &logGroupName)
        {
            spdlog::drop(logGroupName);
        }

        void registerLogSink(LogSink sink);
    };
}
#define JN_TRACE(logGroup, ...) Log::dispatchLog(logGroup, spdlog::level::trace, __VA_ARGS__)
#define JN_DEBUG(logGroup, ...) Log::dispatchLog(logGroup, spdlog::level::debug, __VA_ARGS__)
#define JN_INFO(logGroup, ...) Log::dispatchLog(logGroup, spdlog::level::info, __VA_ARGS__)
#define JN_WARN(logGroup, ...) Log::dispatchLog(logGroup, spdlog::level::warn, __VA_ARGS__)
#define JN_ERROR(logGroup, ...) Log::dispatchLog(logGroup, spdlog::level::err, __VA_ARGS__)
#define JN_CRITICAL(logGroup, ...) Log::dispatchLog(logGroup, spdlog::level::critical, __VA_ARGS__)
