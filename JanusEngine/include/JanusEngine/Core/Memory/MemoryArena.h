#pragma once

#include "Core/Memory/Utils.h"

namespace JanusEngine {
    template <class AllocationPolicy, class ThreadPolicy, class BoundsCheckingPolicy, class MemoryTrackingPolicy, class MemoryTaggingPolicy>
    class MemoryArena
    {
    public:
        template <class AreaPolicy>
        explicit MemoryArena(AreaPolicy& area)
            : allocator(area.GetStart(), area.GetEnd())
        {
        }
    
        void* Allocate(size_t size, size_t alignment, SourceInfo sourceInfo)
        {
            threadGuard.Enter();
    
            const size_t originalSize = size;
            const size_t newSize = size + BoundsCheckingPolicy::SIZE_FRONT + BoundsCheckingPolicy::SIZE_BACK;
    
            byte* plainMemory = static_cast<byte*>(allocator.Allocate(newSize, alignment, BoundsCheckingPolicy::SIZE_FRONT));
            boundsChecker.GuardFront(plainMemory);
            memoryTagger.TagAllocation(plainMemory + BoundsCheckingPolicy::SIZE_FRONT, originalSize);
            boundsChecker.GuardBack(plainMemory + BoundsCheckingPolicy::SIZE_FRONT + originalSize);
            
            sourceInfo.size = newSize;
            memoryTracker.OnAllocation(plainMemory, newSize, alignment, sourceInfo);
    
            threadGuard.Leave();
    
            return (plainMemory + BoundsCheckingPolicy::SIZE_FRONT);
        }
    
        void Free(void* ptr)
        {
            threadGuard.Enter();
    
            byte* originalMemory = static_cast<byte*>(ptr) - BoundsCheckingPolicy::SIZE_FRONT;
            const size_t allocationSize = allocator.GetAllocationSize(originalMemory);
    
            boundsChecker.CheckFront(originalMemory);
            boundsChecker.CheckBack(originalMemory + allocationSize - BoundsCheckingPolicy::SIZE_BACK);
    
            memoryTracker.OnDeallocation(originalMemory);
            
            memoryTagger.TagDeallocation(originalMemory, allocationSize);
    
            allocator.Free(originalMemory);
    
            threadGuard.Leave();
        }
    
    public:
    AllocationPolicy allocator;
    ThreadPolicy threadGuard;
    BoundsCheckingPolicy boundsChecker;
    MemoryTrackingPolicy memoryTracker;
    MemoryTaggingPolicy memoryTagger;
    };
}