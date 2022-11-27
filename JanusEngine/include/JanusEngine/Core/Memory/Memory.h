#pragma once
#include "Core/Core.h"
#include "Core/Memory/Utils.h"
#include "Core/Memory/Allocators.h"
#include "Core/Memory/MemoryArena.h"
#include "Core/Memory/MemoryArea.h"

#include "Core/Memory/BoundsCheckingPolicy.h"
#include "Core/Memory/TrackingPolicy.h"
#include "Core/Memory/TaggingPolicy.h"
#include "Core/Memory/ThreadPolicy.h"

namespace JanusEngine 
{
  #define JN_NEW(type, arena)    new (arena.Allocate(sizeof(type), alignof(type), SourceInfo{__FILE__, __LINE__})) type

  template <typename T, class ARENA>
  void Delete(T* object, ARENA& arena)
  {
      object->~T();
      arena.Free(object);
  }

  #define JN_DELETE(object, arena)    Delete(object, arena)


  template <typename T>
  struct IsPOD
  {
    static const bool Value = std::is_pod<T>::value;
  };

  template <bool I>
  struct IntToType
  {
  };
  
  typedef IntToType<false> NonPODType;
  typedef IntToType<true> PODType;

  template <typename T, class ARENA>
  T* NewArray(ARENA& arena, size_t N, const byte* file, int line, NonPODType)
  {
    union
    {
      void* as_void;
      size_t* as_size_t;
      T* as_T;
    };
  
    as_void = arena.Allocate(sizeof(T)*N + sizeof(size_t), alignof(T), SourceInfo{file, line});
  
    // store number of instances in first size_t bytes
    *as_size_t++ = N;
  
    // construct instances using placement new
    const T* const onePastLast = as_T + N;
    while (as_T < onePastLast)
      new (as_T++) T;
  
    // hand user the pointer to the first instance
    return (as_T - N);
  }
  
  template <typename T, class ARENA>
  T* NewArray(ARENA& arena, size_t N, const byte* file, int line, PODType)
  {
    return (T*)(arena.Allocate(sizeof(T)*N, alignof(T), SourceInfo{file, line}));
  }
  
  template <typename T, class ARENA>
  void DeleteArray(T* ptr, ARENA& arena, NonPODType)
  {
    union
    {
      size_t* as_size_t;
      T* as_T;
    };
  
    // user pointer points to first instance...
    as_T = ptr;
  
    // ...so go back size_t bytes and grab number of instances
    const size_t N = as_size_t[-1];
  
    // call instances' destructor in reverse order
    for (size_t i=N; i>0; --i)
      as_T[i-1].~T();
  
    arena.Free(as_size_t-1);
  }
  
  template <typename T, class ARENA>
  void DeleteArray(T* ptr, ARENA& arena, PODType)
  {
    arena.Free(ptr);
  }

  template <typename T, class ARENA>
  void DeleteArray(T* ptr, ARENA& arena)
  {
    DeleteArray(ptr, arena, IntToType<IsPOD<T>::Value>());
  }

  #define JN_NEW_ARRAY(type, arena)    NewArray<TypeAndCount<type>::Type>(arena, TypeAndCount<type>::Count, __FILE__, __LINE__, IntToType<IsPOD<TypeAndCount<type>::Type>::Value>())

  #define JN_DELETE_ARRAY(object, arena)    DeleteArray(object, arena)

  // ----------------------------------------- MEMORY AREAS ---------------------------------------
}

