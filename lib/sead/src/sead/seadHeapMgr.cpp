#include "sead/seadHeapMgr.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeap.h"
#include "sead/seadThread.h"
#include "sead/seadThreadMgr.h"

namespace sead {
// the marker of destroyed disposers (a variable of the file, next to the instance pointer)
// 0x00976354
sead::Heap* sead::IDisposer::s_pDestructedHeap = reinterpret_cast<Heap*>(1);
// 0x00976358
sead::HeapMgr* sead::HeapMgr::s_pInstance = nullptr;
// 0x0097635C
sead::Arena* sead::HeapMgr::s_pArena = nullptr;
// 0x00976360
sead::AtomicCounter sead::HeapMgr::s_HeapIdCounter;
// 0x00976364
sead::HeapMgr sead::HeapMgr::s_Instance;
// 0x00AE65B8
sead::Arena sead::HeapMgr::s_DefaultArena;
// 0x00AE65C4
sead::FixedPtrArray<sead::Heap, 4> sead::HeapMgr::s_RootHeaps;
// 0x00AE65E0
sead::CriticalSection sead::HeapMgr::s_HeapTreeLockCS;
// 0x00AE65FC
sead::FixedPtrArray<sead::Heap, 4> sead::HeapMgr::s_IndependentHeaps;

// the globals of the file: the instance, the arena, the root heaps and the lock of the heap tree
// 0x00793088 (static initializer)

// 0x0055F318 slot 0x04
// 0x0055F314 (deleting dtor)
sead::HeapMgr::~HeapMgr()
{
    // empty (in the original too)
}

// 0x00133624 (name is ours)
void sead::HeapMgr::initialize(Arena* arena)
{
    s_pArena = arena;
    s_Instance.mAllocFailedCallback = nullptr;
    SafeStringBase<char> name("RootHeap");
    Heap* root = ExpHeap::CreateRoot(arena->mStart, arena->mSize, &name, false);
    s_RootHeaps.pushBack(root);
    s_pInstance = &s_Instance;
}

// 0x001336A0 (name is ours)
void sead::HeapMgr::initialize(u32 size)
{
    ScopedLock lock(getHeapTreeLockCS_());
    s_pArena = &s_DefaultArena;
    s_DefaultArena.initialize(size);
    s_Instance.mAllocFailedCallback = nullptr;
    SafeStringBase<char> name("RootHeap");
    Heap* root = ExpHeap::CreateRoot(s_pArena->mStart, s_pArena->mSize, &name, false);
    s_RootHeaps.pushBack(root);
    s_pInstance = &s_Instance;
}

// 0x00139E30 (name is ours)
sead::Heap* sead::HeapMgr::findContainHeap(const void* ptr) const
{
    u32* cache = nullptr;
    if (ThreadMgr::s_pInstance != nullptr) {
        Thread* thread = ThreadMgr::s_pInstance->getCurrentThread();
        if (thread != nullptr) {
            cache = &thread->mFindContainHeapCache;
            Heap* heap = reinterpret_cast<Heap*>(__atomic_fetch_or(cache, 1, __ATOMIC_RELAXED));
            if (heap != nullptr && heap->mChildren.mCount == 0 && heap->isInclude(ptr)) {
                __atomic_fetch_and(cache, ~1u, __ATOMIC_RELAXED);
                return heap;
            }
            __atomic_fetch_and(cache, ~1u, __ATOMIC_RELAXED);
        }
    }

    ScopedLock lock(getHeapTreeLockCS_());
    if (cache != nullptr) {
        Heap* cached = reinterpret_cast<Heap*>(*cache);
        if (cached != nullptr) {
            Heap* found = cached->findContainHeap_(ptr);
            if (found != nullptr) {
                if (found != cached) {
                    *cache = reinterpret_cast<u32>(found);
                }
                return found;
            }
        }
    }
    for (Heap** it = s_RootHeaps.begin(); it != s_RootHeaps.end(); it++) {
        Heap* found = (*it)->findContainHeap_(ptr);
        if (found != nullptr) {
            if (cache != nullptr) {
                *cache = reinterpret_cast<u32>(found);
            }
            return found;
        }
    }
    for (Heap** it = s_IndependentHeaps.begin(); it != s_IndependentHeaps.end(); it++) {
        Heap* found = (*it)->findContainHeap_(ptr);
        if (found != nullptr) {
            if (cache != nullptr) {
                *cache = reinterpret_cast<u32>(found);
            }
            return found;
        }
    }
    return nullptr;
}

// 0x0012CD58 | nintendogs:callseq-callee [tier A]
sead::Heap* sead::HeapMgr::setCurrentHeap_(Heap* heap)
{
    Thread* thread = ThreadMgr::s_pInstance->getCurrentThread();
    Heap* prev = thread->mCurrentHeap;
    thread->mCurrentHeap = heap;
    return prev;
}

// 0x003047C8 | libgarden [tier A]
sead::Heap* sead::HeapMgr::getCurrentHeap() const
{
    return ThreadMgr::s_pInstance->getCurrentThread()->mCurrentHeap;
}

namespace {
// clears the findContainHeap cache of a thread if it holds heap; false while the thread uses heap
bool removeFromCache(u32* cache, Heap* heap)
{
    u32 expected = reinterpret_cast<u32>(heap);
    if (__atomic_compare_exchange_n(cache, &expected, 0, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
        return true;
    }
    return (expected & ~1u) != reinterpret_cast<u32>(heap);
}
} // namespace

// 0x0055F1B4 (name is ours)
void sead::HeapMgr::removeFromFindContainHeapCache_(Heap* heap)
{
    ThreadMgr* mgr = ThreadMgr::s_pInstance;
    if (mgr == nullptr) {
        return;
    }
    Thread* mainThread = mgr->mMainThread;
    if (mainThread != nullptr) {
        while (!removeFromCache(&mainThread->mFindContainHeapCache, heap)) {
            Thread::sleep(TickSpan::fromMicroSeconds(10));
        }
    }
    for (;;) {
        bool isBusy = false;
        mgr->mListCS.lock();
        for (TListNode<Thread*>* node = mgr->mList.front(); node != mgr->mList.end(); node = TList<Thread*>::next(node)) {
            if (!removeFromCache(&node->mData->mFindContainHeapCache, heap)) {
                isBusy = true;
                break;
            }
        }
        mgr->mListCS.unlock();
        if (!isBusy) {
            return;
        }
        Thread::sleep(TickSpan::fromMicroSeconds(10));
    }
}

} // namespace sead
