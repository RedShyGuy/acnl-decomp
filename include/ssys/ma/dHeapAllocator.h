#pragma once

#include "decomp.h"
#include "ssys/ma/dAllocator.h"
#include "sead/seadHeap.h"

namespace sead {
template <typename T>
class SafeStringBase;
} // namespace sead

namespace ssys {
namespace ma {
// an Allocator that owns its heap (function names are ours)
// RTTI N4ssys2ma13HeapAllocatorE @ 0x008D2434
// vtable 0x00907244 (vptr 0x0090724C), offset_to_top 0, 4 entries
class HeapAllocator : public ::ssys::ma::Allocator
{
public:
    HeapAllocator(); // 0x00567EB0
    virtual ~HeapAllocator(); // 0x00317FD4 slot 0x00

    // replaces the heap by a new FrameHeap
    bool CreateFrameHeap(u32 size, sead::Heap* parent, const sead::SafeStringBase<char>& name,
                         sead::Heap::HeapDirection direction); // 0x00567E50
    // replaces the heap by a new ExpHeap
    bool CreateExpHeap(u32 size, sead::Heap* parent, const sead::SafeStringBase<char>& name,
                       sead::Heap::HeapDirection direction, bool enableLock); // 0x00317B74
    // adjusts the heap if it is an ExpHeap
    u32 Adjust(); // 0x00567DC8
    // destroys the heap
    DECOMP_EMIT_COPY void DestroyHeap() // 0x002F72D4
    {
        if (m_pHeap != NULL) {
            m_pHeap->destroy();
            m_pHeap = NULL;
        }
    }
};
ASSERT_SIZE(HeapAllocator, 0x8);
} // namespace ma
} // namespace ssys
