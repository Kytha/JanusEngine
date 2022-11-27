#include <gtest/gtest.h>
#include "Core/Memory/Memory.h"
namespace JanusEngine
{
    TEST(MemoryArenaTest, BasicAllocation)
    {
        HeapArea area(1024);
        MemoryArena<LinearAllocator, SingleThreadPolicy, NoBoundsChecking, MemoryTrackingWithSourceInfo, NoMemoryTagging> arena(area);

        struct testStruct
        {
            int x;
            int y;
            int z;
            char a;
        };

        testStruct* struct1 = JN_NEW(testStruct, arena);    
        EXPECT_EQ(arena.memoryTracker.GetNumberOfAllocations(),1);
        EXPECT_EQ(arena.memoryTracker.GetNumOfBytesAllocated(), sizeof(testStruct));
    }
}