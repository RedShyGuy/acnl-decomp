#include "sead/seadShrinkBuffer.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"

namespace sead {
// 0x00549FD4 (name is ours)
void sead::ShrinkBuffer::allocate(Heap* heap, u32 size)
{
    if (heap == nullptr) {
        heap = HeapMgr::s_pInstance->getCurrentHeap();
    }
    if (size == 0) {
        size = heap->getMaxAllocatableSize(4);
    }
    mBuffer = static_cast<u32*>(heap->tryAlloc(size, 4));
}

// 0x0054A03C (name is ours)
s32 sead::ShrinkBuffer::shrink(Heap* heap, u32* end)
{
    if (heap == nullptr) {
        heap = HeapMgr::s_pInstance->getCurrentHeap();
    }
    mWordNum = end - mBuffer;
    mBuffer = static_cast<u32*>(heap->resizeBack(mBuffer, mWordNum * sizeof(u32)));
    return mWordNum;
}
} // namespace sead
