#include "Core/Memory/Memory.h"

namespace JanusEngine
{
    byte* alignForward(byte* ptr, size_t align) 
    {
        uintptr_t mod;
        mod = (uintptr_t)ptr % align;
        if (mod != 0) {
            ptr = (byte*)((uintptr_t)ptr + align - mod);
        }
        return ptr;
    }
}