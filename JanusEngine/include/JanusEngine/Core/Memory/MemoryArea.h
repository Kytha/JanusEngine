#pragma once
#include "Core/Core.h"

class HeapArea
{
public:
    HeapArea(size_t size)
    {
        start = (byte*)malloc(size * sizeof(char));
        end = start + size;
        memory = size;
    }
    ~HeapArea()
    {
        free(start);
    }
 
    byte* const GetStart() { return start; }
    byte* const GetEnd() { return end; }
    size_t GetMemory() { return memory; }
 
private:
    byte* start;
    byte* end;
    size_t memory;
};