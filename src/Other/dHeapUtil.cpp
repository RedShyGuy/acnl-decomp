#include "Other/dHeapUtil.h"

#include "sead/seadExpHeap.h"

namespace HeapUtil {
// 0x0011EE4C (name is ours)
sead::ExpHeap* CreateExpHeapWithCName(u32 size, sead::Heap* parent, const char* name, s32)
{
    return sead::ExpHeap::create(size, name, parent, sead::Heap::HEAP_DIRECTION_FORWARD, false);
}

// 0x0011D918 (name is ours)
u32 AdjustHeap(sead::Heap* heap)
{
    if (heap == NULL) {
        return 0;
    }
    return heap->adjust();
}
} // namespace HeapUtil
