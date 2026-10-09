#include "nn/init/init_Api.h"
#include <new>
#include "nn/fnd/fnd_ExpHeapTemplate.h"
#include "nn/os/os_MemoryBlock.h"

namespace nn {
namespace init {
typedef nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> > Heap;

// the heap of malloc / free (used by the C library; names are ours)
struct AllocatorState
{
    Heap* pHeap;
    nn::fnd::IAllocator* pAllocator;
};

// (raw memory: nnosMemoryBlockAllocate and InitializeAllocator make the objects in place, no
// static initializer or destructor touches them)
// 0x00AE1F04
u64 s_HeapMemory[(sizeof(nn::os::MemoryBlock) + 7) / 8];
// 0x00AE1EF8
u64 s_HeapAllocator[(sizeof(Heap::Allocator) + 7) / 8];
// 0x00975F48
AllocatorState s_AllocatorState;

// 0x0011D56C | mk7dlp:bytes [tier B]
void InitializeAllocator(size_t size)
{
    nn::os::MemoryBlock* pBlock = reinterpret_cast<nn::os::MemoryBlock*>(s_HeapMemory);
    nnosMemoryBlockAllocate(pBlock, size);
    InitializeAllocator(pBlock->GetAddress(), size);
}

// 0x0011E4DC | tier C
void InitializeAllocator(uptr address, size_t size)
{
    // the heap object at the start (aligned to 4), its memory after it
    uptr heapAddress = ((address - 1) / 4) * 4 + 4;
    Heap* pHeap = NULL;
    if (heapAddress != 0) {
        pHeap = new (reinterpret_cast<void*>(heapAddress)) Heap;
        pHeap->Initialize(heapAddress + sizeof(Heap), (address + size) - (heapAddress + sizeof(Heap)), 0);
    }
    s_AllocatorState.pHeap = pHeap;
    Heap::Allocator* pAllocator = new (s_HeapAllocator) Heap::Allocator;
    pAllocator->Initialize(pHeap);
    s_AllocatorState.pAllocator = pAllocator;
}

} // namespace init
} // namespace nn
