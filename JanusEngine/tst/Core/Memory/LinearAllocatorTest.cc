#include <gtest/gtest.h>
#include "Core/Memory/Memory.h"
namespace JanusEngine
{
    TEST(LinearAllocator, SinlgePODAllocation)
    {
        int TEST_DATA = 10;
        int HEAP_SIZE = 1024;

        HeapArea area(HEAP_SIZE);
        LinearAllocator allocator(area.GetStart(), area.GetEnd());

        EXPECT_EQ(allocator.GetFreeSpace(), 1024);
        EXPECT_EQ(allocator.GetUsedSpace(), 0);

        struct TestStruct
        {
            int x;
            int y;
            int z;
            char a;
        };
        TestStruct* allocation = new (allocator.Allocate(sizeof(TestStruct), alignof(TestStruct), 0)) TestStruct; 
        allocation->x = TEST_DATA;
        allocation->y = TEST_DATA + 1;
        allocation->z = TEST_DATA + 2;

        EXPECT_EQ(allocator.GetAllocationSize((byte*)allocation),sizeof(TestStruct));
        EXPECT_EQ(allocator.GetUsedSpace(), sizeof(TestStruct) + sizeof(size_t));

        EXPECT_EQ(allocation->x, TEST_DATA);
        EXPECT_EQ(allocation->y, TEST_DATA + 1);
        EXPECT_EQ(allocation->z, TEST_DATA + 2);
        
        allocator.Free((byte*)(allocation));

        EXPECT_EQ(allocator.GetUsedSpace(), sizeof(TestStruct) + sizeof(size_t));

        allocator.Reset();

        EXPECT_EQ(allocator.GetUsedSpace(), 0);
    }

    TEST(LinearAllocator, ArrayPODAllocation)
    {
        int TEST_DATA = 10;
        int HEAP_SIZE = 2048;
        int ARRAY_SIZE = 100;

        HeapArea area(HEAP_SIZE);
        LinearAllocator allocator(area.GetStart(), area.GetEnd());

        struct TestStruct
        {
            int x;
            int y;
            int z;
            char a;
        };

        TestStruct* allocation = (TestStruct*) allocator.Allocate(sizeof(TestStruct)*ARRAY_SIZE, alignof(TestStruct),0);
        EXPECT_EQ(allocator.GetUsedSpace(), sizeof(TestStruct) * ARRAY_SIZE + sizeof(size_t));

        for (size_t i = 0; i < ARRAY_SIZE; i++) 
        {
            allocation[i].x = i;
            allocation[i].y = i + 1;
            allocation[i].z = i + 2;
        }

        for (size_t i = 0; i < ARRAY_SIZE; i++) 
        {
            EXPECT_EQ(allocation[i].x, i);
            EXPECT_EQ(allocation[i].y, i + 1);
            EXPECT_EQ(allocation[i].z, i + 2);
        }

        allocator.Reset();

        EXPECT_EQ(allocator.GetUsedSpace(), 0);
    }

    TEST(LinearAllocator, SingleNonPODAllocation)
    {
        class TestClass
        {
            public:
                TestClass(int a, int b, int c)
                    : a(a)
                    , b(b)
                    , c(c)
                {
                    initialized = true;
                }
                ~TestClass() 
                {
                    initialized = false;
                }
                int GetA() {return a;}
                int GetB() {return b;}
                int GetC() {return c;}
                bool IsInitialized() {return initialized;}
            private:
                int a = 0;
                int b = 0;
                int c = 0;
                bool initialized = false;
        };

        int HEAP_SIZE = 1024;
        int TEST_DATA = 10;

        HeapArea area(HEAP_SIZE);
        LinearAllocator allocator(area.GetStart(), area.GetEnd());

        TestClass* allocation = new (allocator.Allocate(sizeof(TestClass), alignof(TestClass), 0)) TestClass(TEST_DATA, TEST_DATA +1, TEST_DATA+2); 

        EXPECT_EQ(allocator.GetAllocationSize((byte*)allocation),sizeof(TestClass));
        EXPECT_EQ(allocator.GetUsedSpace(), sizeof(TestClass) + sizeof(size_t));

        EXPECT_EQ(allocation->GetA(), TEST_DATA);
        EXPECT_EQ(allocation->GetB(), TEST_DATA + 1);
        EXPECT_EQ(allocation->GetC(), TEST_DATA + 2);
        EXPECT_TRUE(allocation->IsInitialized());

        allocation->~TestClass();
        allocator.Free((byte*)allocation);

        EXPECT_FALSE(allocation->IsInitialized());

        allocator.Reset();
        EXPECT_EQ(allocator.GetUsedSpace(), 0);
    }
}