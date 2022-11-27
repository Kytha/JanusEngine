#pragma once

namespace JanusEngine
{
    class NoMemoryTagging
    {
    public:
    inline void TagAllocation(void*, size_t) const {}
    inline void TagDeallocation(void*, size_t) const {}
    };
}