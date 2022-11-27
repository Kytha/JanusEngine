#pragma once
#include "Core/Core.h"

namespace JanusEngine
{
    byte* alignForward(byte* ptr, size_t align);
    
    struct SourceInfo
    {
    const unsigned char* file;
    int line;
    size_t size;
    };
}