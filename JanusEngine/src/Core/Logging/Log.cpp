#include "Core/Logging/Log.h"

namespace JanusEngine
{
    void Log::registerLogGroup(LogGroupProps logGroupProps)
    {
        try
        {
            std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(logGroupProps.name);
            logger->set_pattern(logGroupProps.formatString);
            logger->set_level(spdlog::level::trace);
        }
        catch (const spdlog::spdlog_ex &ex)
        {
            std::cout << "Log initialization failed: " << ex.what() << std::endl;
        }
    }
}