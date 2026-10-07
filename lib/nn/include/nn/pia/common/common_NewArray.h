#pragma once

// Arrays on the current pia heap without a cookie: the objects are value initialized one by one;
// on deletion their number comes from the size of the memory block (pead::GetMemoryBlockInfo,
// called even when the destructor is trivial). Inline everywhere; the file and function names are
// ours.

#include "decomp.h"
#include "nn/pia/common/common_HeapManager.h"
#include "pead/peadHeapMgr.h"
#include <new>

namespace nn {
namespace pia {
namespace common {
template <typename T>
DECOMP_ALWAYS_INLINE T* NewArray(u32 num)
{
    T* p = static_cast<T*>(pead::AllocMemory(num * sizeof(T), HeapManager::GetHeap()));
    if (p != nullptr) {
        for (u32 i = 0; i < num; i++) {
            ::new (&p[i]) T();
        }
    }
    return p;
}

// the same with an alignment (operator new(size, heap, alignment))
template <typename T>
DECOMP_ALWAYS_INLINE T* NewArray(u32 num, int alignment)
{
    T* p = static_cast<T*>(::operator new(num * sizeof(T), HeapManager::GetHeap(), alignment));
    if (p != nullptr) {
        for (u32 i = 0; i < num; i++) {
            ::new (&p[i]) T();
        }
    }
    return p;
}

template <typename T>
DECOMP_ALWAYS_INLINE void DeleteArray(T* p)
{
    u32 num = pead::GetMemoryBlockInfo(p) / sizeof(T);
    for (u32 i = 0; i < num; i++) {
        p[i].~T();
    }
    pead::FreeMemory(p);
}
// one object on the current pia heap (null if the heap is full), and its release with the
// virtual destructor (pia local; names are ours)
template <typename T>
DECOMP_ALWAYS_INLINE T* NewObject()
{
    void* p = pead::AllocMemory(sizeof(T), HeapManager::GetHeap());
    return ::new (p) T();
}

template <typename T>
DECOMP_ALWAYS_INLINE void DeleteObject(T* p)
{
    p->~T();
    pead::FreeMemory(p);
}
} // namespace common
} // namespace pia
} // namespace nn
