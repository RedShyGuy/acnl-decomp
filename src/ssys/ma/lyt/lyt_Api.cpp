#include "ssys/ma/lyt/lyt_Api.h"
#include "ssys/ma/dAllocator.h"
#include "Other/dHeapUtil.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"

namespace ssys {
namespace ma {
namespace lyt {
// (names are ours)
// 0x00976C44
Allocator* s_pAllocator = NULL;
// 0x00976C48
Allocator* s_pDeviceAllocator = NULL;

// 0x0056A838 | libgarden [tier A]
Allocator* GetAllocator()
{
    return s_pAllocator;
}

// 0x0056A8AC (name after libgarden)
Allocator* GetDeviceAllocator()
{
    return s_pDeviceAllocator;
}

// 0x0056A8BC (name is ours)
void InitializeHeap(u32 size, sead::Heap* parent)
{
    sead::ExpHeap* heap = HeapUtil::CreateExpHeapWithCName(size, parent, "2D_UI_Heap", 4);
    heap->mFlag |= sead::Heap::FLAG_ENABLE_LOCK;
    s_pAllocator = new (heap, 4) Allocator();
    s_pAllocator->SetHeap(heap);
    s_pDeviceAllocator = new (heap, 4) Allocator();
    s_pDeviceAllocator->SetHeap(heap);
}

} // namespace lyt
} // namespace ma
} // namespace ssys
