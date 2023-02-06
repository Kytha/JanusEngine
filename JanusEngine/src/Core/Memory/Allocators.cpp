#include "Core/Memory/memory.h"

namespace JanusEngine
{
    LinearAllocator::LinearAllocator(byte *start, byte *end)
        : cursor(start), start(start), end(end)
    {
    }

    byte *LinearAllocator::Allocate(size_t size, size_t alignment, size_t offset)
    {
        // offset pointer first, align it, and offset it back
        cursor = alignForward(cursor + offset, alignment) - offset;

        // Store size of allocation sizeof(size_t) bytes before user data
        union
        {
            void *as_void;
            size_t *as_size_t;
        };
        as_void = cursor;
        *as_size_t = size;

        cursor += sizeof(size);

        byte *userPtr = cursor;
        cursor += size;

        if (cursor >= end)
        {
            // Out of memory
            return nullptr;
        }

        return userPtr;
    }
}