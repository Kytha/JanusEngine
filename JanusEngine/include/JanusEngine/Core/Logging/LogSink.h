#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"

namespace JanusEngine
{
    class LogSink : public spdlog::sinks::base_sink<spdlog::details::null_mutex>
    {
    public:
        LogSink(std::initializer_list<std::string> logGroups, bool isGlobal = true);
        bool isGlobal = false;
        std::vector<std::string> logGroupNames;

    protected:
        void sink_it_(const spdlog::details::log_msg &msg) override;

        void flush_() override;
    };
}