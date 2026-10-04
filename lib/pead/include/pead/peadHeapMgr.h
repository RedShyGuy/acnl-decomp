#pragma once

#include "decomp.h"
#include "pead/hostio/peadNode.h"
#include "pead/peadPtrArray.h"

namespace pead {
class Heap;

// The memory the root heap is made in (pia's common::HeapManager::Initialize fills one and passes
// it to HeapMgr::initialize; the names are ours)
struct Arena
{
    void* mStart;       // 0x0
    u32 mSize;          // 0x4
    bool mInitialized;  // 0x8
};
ASSERT_SIZE(Arena, 0xC);

// RTTI N4pead7HeapMgrE @ 0x008D12F8
// vtable 0x00904D9C (vptr 0x00904DA4), offset_to_top 0, 2 entries
class HeapMgr : public ::pead::hostio::Node
{
public:
    HeapMgr(); // ctor candidate(s) 0x00793140 (unverified)
    virtual void vf_0x00(); // 0x0053D80C slot 0x00 | virtual slot, introduced by pead::HeapMgr
    virtual void vf_0x04(); // 0x0053D808 slot 0x04 | virtual slot, introduced by pead::HeapMgr

    // makes the root heap "RootHeap" in the arena (name is ours)
    static void initialize(Arena* arena); // 0x0053D694
    // destroys the root heaps again (name is ours)
    static void destroy(); // 0x0053D710

    static Heap* getRootHeap(int index) { return sRootHeaps.at(index); }

    static PtrArray<Heap> sRootHeaps; // 0x00AE82B4 (name is ours)
};

// allocation with a heap (the current one if heap is null) and release to the heap that contains
// the memory; pia's RootObject::operator new / delete call them (names are ours)
void* AllocMemory(size_t size, Heap* heap); // 0x0053B2F8
void FreeMemory(void* ptr); // 0x005387F8
// a value from the header of an allocated block (pia's CachedPrint calls it before it frees its
// buffer and ignores the result; name is ours)
u32 GetMemoryBlockInfo(const void* ptr); // 0x00538754
} // namespace pead
