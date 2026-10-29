#include "pead/peadExpHeap.h"
#include "pead/peadHeap.h"
#include "pead/peadHeapMgr.h"

// The allocation helpers of pead (the name of the file is ours).

namespace pead {
// 0x00538754 (name is ours)
u32 GetMemoryBlockInfo(const void* ptr)
{
    return MemBlock::fromMemory(const_cast<void*>(ptr))->mSize;
}

// 0x005387F8 (name is ours)
void FreeMemory(void* ptr)
{
    Heap* heap = HeapMgr::s_pInstance->findContainHeap(ptr);
    if (heap != nullptr) {
        heap->free(ptr);
    }
}

// 0x0053B2F8 (name is ours)
void* AllocMemory(size_t size, Heap* heap)
{
    if (heap == nullptr) {
        heap = HeapMgr::s_pInstance->getCurrentHeap();
        if (heap == nullptr) {
            return nullptr;
        }
    }
    return heap->tryAlloc(size, 4);
}
} // namespace pead

// 0x0053B33C | nintendogs:bytes-fuzzy [tier A]
void* operator new(size_t size, pead::Heap* heap, int alignment)
{
    if (heap == nullptr) {
        heap = pead::HeapMgr::s_pInstance->getCurrentHeap();
        if (heap == nullptr) {
            return nullptr;
        }
    }
    return heap->tryAlloc(size, alignment);
}
