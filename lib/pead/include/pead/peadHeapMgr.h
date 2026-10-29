#pragma once

#include "decomp.h"
#include "pead/hostio/peadNode.h"
#include "pead/peadArena.h"
#include "pead/peadCriticalSection.h"
#include "pead/peadIDelegate1.h"
#include "pead/peadPtrArray.h"

#include <cstddef>
#include <new>

namespace pead {
class Heap;

// RTTI N4pead7HeapMgrE @ 0x008D12F8
// vtable 0x00904D9C (vptr 0x00904DA4), offset_to_top 0, 2 entries
//
// pia's copy of sead::HeapMgr: the root heaps (made in the arena) and the current heap of each thread (without
// the per-thread cache of sead's findContainHeap). getCurrentHeap is from the reference symbols; the other names
// are ours or those of our sead.
class HeapMgr : public ::pead::hostio::Node
{
public:
    // what ExpHeap::tryAlloc passes to the callback when an allocation fails (names are ours)
    struct AllocFailedCallbackArg
    {
        Heap* heap;
        u32 requestSize;
        s32 requestAlignment;
        u32 allocSize;
        s32 allocAlignment;
    };
    typedef IDelegate1<const AllocFailedCallbackArg*> AllocFailedCallback;

    HeapMgr() : mAllocFailedCallback(nullptr) {}
    virtual ~HeapMgr(); // 0x0053D80C slot 0x00
    // 0x0053D808 slot 0x04 (deleting dtor)

    // makes the root heap "RootHeap" in the arena (name is ours)
    static void initialize(Arena* arena); // 0x0053D694
    // destroys the independent and the root heaps and forgets the arena (name is ours)
    static void destroy(); // 0x0053D710
    // the heap that contains ptr (the deepest one), NULL if there is none (name after sead)
    Heap* findContainHeap(const void* ptr) const; // 0x00749CE8
    Heap* getCurrentHeap() const; // 0x00749CC8 | nintendogs:bytes [tier A]

    static Heap* getRootHeap(int index) { return s_RootHeaps.at(index); }
    static CriticalSection* getHeapTreeLockCS_() { return &s_HeapTreeLockCS; }

    static HeapMgr* s_pInstance;
    static Arena* s_pArena;
    static HeapMgr s_Instance;
    static Arena s_DefaultArena;
    static FixedPtrArray<Heap, 4> s_RootHeaps;
    static CriticalSection s_HeapTreeLockCS;
    static FixedPtrArray<Heap, 4> s_IndependentHeaps;

    AllocFailedCallback* mAllocFailedCallback; // 0x4
};
ASSERT_SIZE(HeapMgr, 0x8);

// allocation with a heap (the current one if heap is null, alignment 4) and release to the heap that contains
// the memory; pia's RootObject::operator new / delete call them (names are ours)
void* AllocMemory(size_t size, Heap* heap); // 0x0053B2F8
void FreeMemory(void* ptr); // 0x005387F8
// the size of an allocated block (from its header; the callers of FreeMemory call it first and ignore the
// result, pia's PacketStream counts its packets with it; name is ours)
u32 GetMemoryBlockInfo(const void* ptr); // 0x00538754

// Arrays on a heap without a cookie (as pia's common::NewArray): the objects are value initialized one by one;
// on deletion their number comes from the size of the memory block (GetMemoryBlockInfo, called even when the
// destructor is trivial). Inline everywhere; the names are ours.
template <typename T>
DECOMP_ALWAYS_INLINE T* NewArray(u32 num, Heap* heap)
{
    T* p = static_cast<T*>(AllocMemory(num * sizeof(T), heap));
    if (p != nullptr) {
        for (u32 i = 0; i < num; i++) {
            ::new (&p[i]) T();
        }
    }
    return p;
}

template <typename T>
DECOMP_ALWAYS_INLINE void DeleteArray(T* p)
{
    u32 num = GetMemoryBlockInfo(p) / sizeof(T);
    for (u32 i = 0; i < num; i++) {
        p[i].~T();
    }
    FreeMemory(p);
}
} // namespace pead

// allocation on a heap with an alignment (the name is from the nintendogs symbols, there for sead)
void* operator new(size_t size, pead::Heap* heap, int alignment); // 0x0053B33C

namespace pead {
// the same with an alignment (operator new(size, heap, alignment); name is ours)
template <typename T>
DECOMP_ALWAYS_INLINE T* NewArray(u32 num, Heap* heap, int alignment)
{
    T* p = static_cast<T*>(::operator new(num * sizeof(T), heap, alignment));
    if (p != nullptr) {
        for (u32 i = 0; i < num; i++) {
            ::new (&p[i]) T();
        }
    }
    return p;
}
} // namespace pead
