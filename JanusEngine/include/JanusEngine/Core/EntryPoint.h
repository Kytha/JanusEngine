#pragma once
#include "Core.h"
namespace JanusEngine
{
    int entryPoint(int argc, const char **argv);
}

#if PLATFORM == PLATFORM_WINDOWS
#define APPLICATION_ENTRY()                  \
    int main(int argc, const char **argv)    \
    {                                        \
        JanusEngine::entryPoint(argc, argv); \
    }
#elif PLATFORM == PLATFORM_LINUX
#define APPLICATION_ENTRY(ApplicationClass, ...) \
    int main(int argc, const char **argv)        \
    {                                            \
        entryPoint(argc, argv);                  \
    }
#elif PLATFORM == PLATFORM_OSX
#define APPLICATION_ENTRY(ApplicationClass, ...) \
    int main(int argc, const char **argv)        \
    {                                            \
        JanusEngine::entryPoint(argc, argv);     \
    }
#endif
