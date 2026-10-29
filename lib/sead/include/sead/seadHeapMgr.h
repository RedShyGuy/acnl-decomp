#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadArena.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadIDelegate1.h"
#include "sead/seadPtrArrayImpl.h"

namespace sead {
class Heap;
template <typename T>
class SafeStringBase;

// an atomic counter (inline; name is ours)
class AtomicCounter
{
public:
    AtomicCounter() : mValue(0) {}
    // the value before the increment
    u32 increment() { return __atomic_fetch_add(&mValue, 1, __ATOMIC_RELAXED); }

    u32 mValue;
};

// RTTI N4sead7HeapMgrE @ 0x008D2144
// vtable 0x00906C80 (vptr 0x00906C88), offset_to_top 0, 3 entries
//
// The root heaps (made in the arena) and the current heap of each thread. The class name and
// setCurrentHeap_/getCurrentHeap are from the reference symbols; the other names are ours.
class HeapMgr : public ::sead::hostio::Node
{
public:
    // what a heap passes to the callback when an allocation fails (names are ours)
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
    virtual ~HeapMgr(); // 0x0055F318 slot 0x04
    // 0x0055F314 slot 0x08 (deleting dtor)

    // makes the root heap in arena (name is ours)
    static void initialize(Arena* arena); // 0x00133624
    // makes the root heap in the device memory (name is ours)
    static void initialize(u32 size); // 0x001336A0
    // the heap that contains ptr (the deepest one), NULL if there is none
    Heap* findContainHeap(const void* ptr) const; // 0x00139E30 (name is ours)
    // makes heap the current heap of the thread; the heap before
    Heap* setCurrentHeap_(Heap* heap); // 0x0012CD58 | nintendogs:callseq-callee [tier A]
    Heap* getCurrentHeap() const; // 0x003047C8 | libgarden [tier A]
    // takes heap out of the findContainHeap caches of all threads (waits while one uses it)
    static void removeFromFindContainHeapCache_(Heap* heap); // 0x0055F1B4 (name is ours)
    static CriticalSection* getHeapTreeLockCS_() { return &s_HeapTreeLockCS; }
    // calls the callback of mgr (if there is one) for a failed allocation (inline; name is ours)
    static void callAllocFailedCallback_(HeapMgr* mgr, Heap* heap, u32 requestSize, s32 requestAlignment, u32 allocSize,
                                         s32 allocAlignment)
    {
        if (mgr != nullptr && mgr->mAllocFailedCallback != nullptr) {
            AllocFailedCallbackArg arg = {heap, requestSize, requestAlignment, allocSize, allocAlignment};
            mgr->mAllocFailedCallback->invoke(&arg);
        }
    }

    static HeapMgr* s_pInstance;
    static HeapMgr s_Instance;
    static Arena* s_pArena;
    static Arena s_DefaultArena;
    static FixedPtrArray<Heap, 4> s_RootHeaps;
    static FixedPtrArray<Heap, 4> s_IndependentHeaps;
    static CriticalSection s_HeapTreeLockCS;
    static AtomicCounter s_HeapIdCounter;

    AllocFailedCallback* mAllocFailedCallback; // 0x4
};
ASSERT_SIZE(HeapMgr, 0x8);

// Makes heap the current heap for its lifetime (nothing for NULL; inline, e.g. in the constructor of
// ptcl::PtclSystem; the name is ours).
class ScopedCurrentHeapSetter
{
public:
    explicit ScopedCurrentHeapSetter(Heap* heap)
    {
        if (heap != nullptr) {
            mPrevHeap = HeapMgr::s_pInstance->setCurrentHeap_(heap);
        } else {
            mPrevHeap = reinterpret_cast<Heap*>(1);
        }
    }
    ~ScopedCurrentHeapSetter()
    {
        if (mPrevHeap != reinterpret_cast<Heap*>(1)) {
            HeapMgr::s_pInstance->setCurrentHeap_(mPrevHeap);
        }
    }

private:
    Heap* mPrevHeap;
};
} // namespace sead
