#pragma once
#include "Core/Core.h"
#include "Core/Memory/Utils.h"

namespace JanusEngine
{
    class NoMemoryTracking
    {
    public:
        inline void OnAllocation(void *, size_t, size_t, const SourceInfo &) const {}
        inline void OnDeallocation(void *) const {}
    };

    class SimpleMemoryTracking
    {
    public:
        inline void OnAllocation(byte *memory, size_t size, size_t alignment, const SourceInfo &sourceInfo) { numOfAllocations++; }
        inline void OnDeallocation(byte *memory) { numOfAllocations--; }
        inline size_t GetNumberOfAllocations() { return numOfAllocations; }

    private:
        size_t numOfAllocations;
    };

    struct MemoryAllocationInfo
    {
        const char *file;
        int line;
        size_t size;
    };

    class MemoryTrackingWithSourceInfo
    {
    public:
        inline void OnAllocation(byte *memory, size_t size, size_t alignment, SourceInfo &sourceInfo)
        {
            allocations.insert(std::make_pair(memory, sourceInfo));
            numOfBytesAllocated += size;
        }

        inline void OnDeallocation(byte *memory)
        {
            std::unordered_map<byte *, SourceInfo>::const_iterator got = allocations.find(memory);
            if (got == allocations.end())
                assert(false);

            numOfBytesAllocated -= got->second.size;
            allocations.erase(memory);
        }

        inline std::unordered_map<byte *, SourceInfo> &GetAllocations() { return allocations; }
        inline size_t GetNumberOfAllocations() { return allocations.size(); }
        inline size_t GetNumOfBytesAllocated() { return numOfBytesAllocated; }

    private:
        std::unordered_map<byte *, SourceInfo> allocations;
        size_t numOfBytesAllocated = 0;
    };
}