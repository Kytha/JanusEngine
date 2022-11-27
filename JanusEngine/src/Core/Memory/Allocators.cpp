#include "Core/Memory/memory.h"

namespace JanusEngine
{
    LinearAllocator::LinearAllocator(byte* start, byte* end)
        : cursor(start)
        , start(start)
        , end(end)
    {
    }

    byte* LinearAllocator::Allocate(size_t size, size_t alignment, size_t offset)
    {
        // offset pointer first, align it, and offset it back
        cursor = alignForward(cursor + offset, alignment) - offset;
        
        byte* userPtr = cursor;
        cursor += size;
        
        if (cursor >= end)
        {
            // out of memory
            std::cout << "Out of memory";
            return nullptr;
        }
        
        return userPtr;
    }
}