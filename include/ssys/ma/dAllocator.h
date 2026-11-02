#pragma once

#include "decomp.h"
#include "nw/os/os_IAllocator.h"

namespace sead {
class Heap;
} // namespace sead

namespace ssys {
namespace ma {
// an nw allocator on a sead heap (member and function names are ours)
// RTTI N4ssys2ma9AllocatorE @ 0x008D2500
// vtable 0x009073AC (vptr 0x009073B4), offset_to_top 0, 4 entries
class Allocator : public ::nw::os::IAllocator
{
public:
    DECOMP_EMIT_COPY DECOMP_NOINLINE Allocator() : m_pHeap(NULL) {} // 0x00120DD0
    virtual ~Allocator(); // 0x0056B23C slot 0x00 | libgarden
    void* Alloc(size_t size, u8 alignment) override; // 0x0056B21C slot 0x08
    void Free(void* p) override; // 0x0056B20C slot 0x0C

    DECOMP_EMIT_COPY DECOMP_NOINLINE bool SetHeap(sead::Heap* heap) // 0x0012D3B8 (name after libgarden)
    {
        m_pHeap = heap;
        return true;
    }

    sead::Heap* m_pHeap; // 0x4
};
ASSERT_SIZE(Allocator, 0x8);
} // namespace ma
} // namespace ssys
