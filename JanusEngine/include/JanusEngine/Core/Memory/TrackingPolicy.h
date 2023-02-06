#pragma once
#include "Core/Core.h"
#include "Core/Memory/Utils.h"

namespace JanusEngine
{
    struct MemoryAllocationInfo
    {
        const char *file;
        int line;
        size_t size;
    };
    class NoMemoryTracking
    {
    public:
        inline void OnAllocation(void *, size_t, size_t, const MemoryAllocationInfo &) const {}
        inline void OnDeallocation(void *) const {}
    };

    class SimpleMemoryTracking
    {
    public:
        inline void OnAllocation(byte *memory, size_t size, size_t alignment, const MemoryAllocationInfo &sourceInfo) { numOfAllocations++; }
        inline void OnDeallocation(byte *memory) { numOfAllocations--; }
        inline size_t GetNumberOfAllocations() { return numOfAllocations; }

    private:
        size_t numOfAllocations;
    };

    class MemoryTrackingWithSourceInfo
    {
    public:
        inline void OnAllocation(byte *memory, size_t size, size_t alignment, MemoryAllocationInfo &sourceInfo)
        {
            allocations.insert(std::make_pair(memory, sourceInfo));
            numOfBytesAllocated += size;
        }

        inline void OnDeallocation(byte *memory)
        {
            std::unordered_map<byte *, MemoryAllocationInfo>::const_iterator got = allocations.find(memory);
            if (got == allocations.end())
                assert(false);

            numOfBytesAllocated -= got->second.size;
            allocations.erase(memory);
        }

        inline std::unordered_map<byte *, MemoryAllocationInfo> &GetAllocations() { return allocations; }
        inline size_t GetNumberOfAllocations() { return allocations.size(); }
        inline size_t GetNumOfBytesAllocated() { return numOfBytesAllocated; }

    private:
        std::unordered_map<byte *, MemoryAllocationInfo> allocations;
        size_t numOfBytesAllocated = 0;
    };
}