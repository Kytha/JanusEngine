#pragma once

// DEBUG 

#if defined(DEBUG) || defined(_DEBUG)
    #define JANUS_DEBUG
#else
    #define JANUS_RELEASE
#endif

// PLATFORM

#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX 2
#define PLATFORM_ANDROID 3
#define PLATFORM_IOS 4
#define PLATFORM_OSX 5

#if defined(_WIN32)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(_WIN64)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined (__CYGWIN)
    #define PLATFORM PLATFORM_WINDOWS
#elif defined(__ANDROID__)
    #define PLATFORM PLATFORM_ANDROID
#elif defined(__linux__)
    #define PLATFORM PLATFORM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #define PLATFORM PLATFORM_IOS
    #elif TARGET_OS_IPHONE == 1
        #define PLATFORM_NAME PLATFORM_IOS // Apple iOS
    #elif TARGET_OS_MAC == 1
        #define PLATFORM PLATFORM_OSX // Apple OSX
    #endif
#else 
    #define PLATFORM_NAME 0
#endif

#if PLATFORM == PLATFORM_WINDOWS
    #include "Windows.h"
#endif

// PROFILER
