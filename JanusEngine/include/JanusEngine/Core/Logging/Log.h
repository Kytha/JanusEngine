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

        static void registerLogGroup(LogGroupProps logGroupProps);

        static inline void deregisterLogGroup(const std::string &logGroupName)
        {
            spdlog::drop(logGroupName);
        }

        void registerLogSink(LogSink sink);
    };
}

#if defined(JANUS_DEBUG)
#define JN_TRACE(logGroup, ...) spdlog::get(logGroup)? spdlog::get(logGroup)->trace(__VA_ARGS__) : 0
#define JN_DEBUG(logGroup, ...) spdlog::get(logGroup)? spdlog::get(logGroup)->debug(__VA_ARGS__) : 0
#else
#define JN_TRACE(logGroup, ...) 0
#define JN_DEBUG(logGroup, ...) 0
#endif

#define JN_INFO(logGroup, ...) spdlog::get(logGroup)? spdlog::get(logGroup)->info(__VA_ARGS__) : 0
#define JN_WARN(logGroup, ...) spdlog::get(logGroup)? spdlog::get(logGroup)->warn(__VA_ARGS__) : 0
#define JN_ERROR(logGroup, ...) spdlog::get(logGroup)? spdlog::get(logGroup)->error(__VA_ARGS__) : 0
#define JN_CRITICAL(logGroup, ...) spdlog::get(logGroup)? spdlog::get(logGroup)->critical(__VA_ARGS__) : 0

#define JN_CORE_LOG_GROUP "CORE"