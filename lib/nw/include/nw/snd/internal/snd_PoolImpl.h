#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// A pool of objects of one size: a list of the free ones, linked through their first word (the class
// and function names are from the reference symbols, the member is ours).
class PoolImpl
{
public:
    // (inline)
    PoolImpl() : m_pNext(nullptr) {}

    // the free objects in the memory; their count
    u32 CreateImpl(void* pMemory, u32 size, u32 objectSize); // 0x004D4A80 | nintendogs:bytes [tier A]
    // the free objects in the memory are taken out
    void DestroyImpl(void* pMemory, u32 size); // 0x004D4AE0 | nintendogs:bytes [tier A]
    // a free object (NULL: none)
    void* AllocImpl(); // 0x004D4B24 | nintendogs:bytes [tier A]
    void FreeImpl(void* p); // 0x004D4B14 | nintendogs:bytes [tier A]
    // the free objects
    int CountImpl() const; // 0x007430A4 | nintendogs:bytes [tier A]

    PoolImpl* m_pNext; // 0x0 the first free object
};
ASSERT_SIZE(PoolImpl, 0x4);
} // namespace internal
} // namespace snd
} // namespace nw
