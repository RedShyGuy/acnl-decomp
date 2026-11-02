#pragma once

#include "decomp.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadINamable.h"

namespace ssys {
namespace ma {
// a heap for VRAM (or other memory the CPU does not use): it hands out addresses, not pointers (function and member
// names are ours, after sead::Heap)
// RTTI N4ssys2ma8VramHeapE @ 0x008D24E8
// vtable 0x00907378 (vptr 0x00907380), offset_to_top 0, 11 entries
class VramHeap : public ::sead::INamable
{
public:
    // (inline in the constructor of VramExpHeap 0x00120C6C)
    explicit VramHeap(const sead::SafeStringBase<char>& name) : sead::INamable(name) {}

    // destroys the heap and the memory it lives in
    virtual void Destroy() = 0;                                       // slot 0x00
    // gives the free end back to the parent; the new size
    virtual u32 Adjust() = 0;                                         // slot 0x04
    // the address of size bytes (0 if there is no room)
    virtual uptr Alloc(u32 size, s32 alignment) = 0;                  // slot 0x08
    virtual void Free(uptr address) = 0;                              // slot 0x0C
    virtual void FreeAll() = 0;                                       // slot 0x10
    virtual u32 GetFreeSize() = 0;                                    // slot 0x14
    virtual u32 GetMaxAllocatableSize(s32 alignment) = 0;             // slot 0x18
    // shrinks the block at address from oldSize to newSize
    virtual bool Resize(uptr address, u32 oldSize, u32 newSize) = 0;  // slot 0x1C
    virtual void Dump() = 0;                                          // slot 0x20
    virtual ~VramHeap();                                              // 0x0056B1F0 slot 0x24

    sead::CriticalSection m_CriticalSection; // 0x0C
};
ASSERT_SIZE(VramHeap, 0x28);
} // namespace ma
} // namespace ssys
