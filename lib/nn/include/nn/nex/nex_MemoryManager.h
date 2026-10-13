#pragma once

#include "decomp.h"
#include <cstdlib>
#include <new>

namespace nn {
namespace nex {
// The memory of nex: every block has an 8 byte header in front of it whose first word is the
// function that frees it (null: free). The application can replace malloc and the free function
// (it writes the two pointers directly). The names of the members are ours.
class MemoryManager
{
public:
    static const u32 HEADER_SIZE = 8;

    // allocates size (rounded up to 4) bytes
    typedef void* (*AllocateFunction)(u32 size);
    // frees a block (it gets the header)
    typedef void (*FreeFunction)(void* pHeader);

    static void* Allocate(u32 size); // 0x00361FA0 | mk7dlp:callgraph [tier A]
    // (name is ours)
    static void Free(void* p); // 0x00361F78
    // Free where the original inlines it (ARMCC inlines across the sources of a blob of nex)
    static DECOMP_ALWAYS_INLINE void FreeInline(void* p)
    {
        if (p == nullptr) {
            return;
        }
        void** pHeader = reinterpret_cast<void**>(static_cast<u8*>(p) - HEADER_SIZE);
        FreeFunction function = reinterpret_cast<FreeFunction>(pHeader[0]);
        if (function != nullptr) {
            function(pHeader);
        } else {
            free(pHeader);
        }
    }

    static AllocateFunction s_AllocateFunction; // null: malloc
    static FreeFunction s_FreeFunction;         // stored in the header of every block, null: free
};

// nex allocates arrays with their element count in a word in front of them (the elements are
// default initialized). The names are ours.
template <typename T>
inline T* NewArray(u32 count)
{
    u32* pBlock = static_cast<u32*>(MemoryManager::Allocate(sizeof(u32) + count * sizeof(T)));
    *pBlock = count;
    T* pArray = reinterpret_cast<T*>(pBlock + 1);
    for (u32 i = 0; i < count; i++) {
        new (&pArray[i]) T;
    }
    return pArray;
}

template <typename T>
inline void DeleteArray(T* pArray)
{
    u32* pBlock = reinterpret_cast<u32*>(pArray) - 1;
    for (u32 i = 0; i < *pBlock; i++) {
        pArray[i].~T();
    }
    MemoryManager::FreeInline(pBlock);
}
} // namespace nex
} // namespace nn
