#pragma once
#include "Core/Core.h"

namespace JanusEngine
{
  class LinearAllocator
  {
  public:
    explicit LinearAllocator(size_t size);
    LinearAllocator(byte *start, byte *end);

    byte *Allocate(size_t size, size_t alignment, size_t offset);

    inline void Free(byte *ptr){};

    inline void Reset(void) { cursor = start; };

    inline size_t GetAllocationSize(byte *allocation) const
    {
      union
      {
        void *as_void;
        char *as_char;
        size_t *as_size_t;
      };

      // grab the allocation's size from the first N bytes before the user data
      as_void = allocation;
      as_char -= sizeof(size_t);
      return (*as_size_t);
    }

    inline size_t GetAvailableSpace()
    {
      return (end - cursor);
    }

    inline size_t GetUsedSpace()
    {
      return (cursor - start);
    }

  private:
    byte *end;
    byte *cursor;
    byte *start;
  };
}