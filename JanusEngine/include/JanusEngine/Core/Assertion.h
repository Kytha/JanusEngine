#pragma once
#include "Core/Core.h"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define JN_ASSERT(logGroupName, condition, format, ...) (condition) ? (void)0 : (Assert(JN_SOURCE_INFO(logGroupName), "ASSERT FAILED @ " __FILE__ ":" STRINGIZE(__LINE__) ": \"" #condition "\" " format, __VA_ARGS__), JN_BREAKPOINT())

namespace JanusEngine
{
    class Assert
    {
    public:
        Assert();
        template <typename... Args>
        Assert(const SourceInfo &sourceInfo, const char *format, Args... args)
        {
            JN_ERROR(sourceInfo.logGroupName, format, args...);
        }
    };
}