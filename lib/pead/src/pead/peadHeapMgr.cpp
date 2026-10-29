#include "pead/peadExpHeap.h"
#include "pead/peadHeap.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadThread.h"
#include "pead/peadThreadMgr.h"

namespace pead {
// 0x0097E42C
pead::HeapMgr* pead::HeapMgr::s_pInstance = nullptr;
// 0x0097E430
pead::Arena* pead::HeapMgr::s_pArena = nullptr;
// 0x0097E434
pead::HeapMgr pead::HeapMgr::s_Instance;
// 0x00AE82A8
pead::Arena pead::HeapMgr::s_DefaultArena;
// 0x00AE82B4
pead::FixedPtrArray<pead::Heap, 4> pead::HeapMgr::s_RootHeaps;
// 0x00AE82D0
pead::CriticalSection pead::HeapMgr::s_HeapTreeLockCS;
// 0x00AE82EC
pead::FixedPtrArray<pead::Heap, 4> pead::HeapMgr::s_IndependentHeaps;

// the globals of the file: the instance, the arena, the root heaps and the lock of the heap tree
// 0x00793140 (static initializer)

// 0x0053D694 (name is ours)
void pead::HeapMgr::initialize(Arena* arena)
{
    s_pArena = arena;
    s_Instance.mAllocFailedCallback = nullptr;
    SafeStringBase<char> name("RootHeap");
    Heap* root = ExpHeap::tryCreate(arena->mStart, arena->mSize, name, false);
    s_RootHeaps.pushBack(root);
    s_pInstance = &s_Instance;
}

// 0x0053D710 (name is ours)
void pead::HeapMgr::destroy()
{
    ScopedLock lock(getHeapTreeLockCS_());
    s_Instance.mAllocFailedCallback = nullptr;
    while (s_IndependentHeaps.size() != 0) {
        s_IndependentHeaps.at(s_IndependentHeaps.size() - 1)->destroy();
        s_IndependentHeaps.popBack();
    }
    while (s_RootHeaps.size() != 0) {
        s_RootHeaps.at(s_RootHeaps.size() - 1)->destroy();
        s_RootHeaps.popBack();
    }
    s_pInstance = nullptr;
    s_pArena->destroy();
    s_pArena = nullptr;
}

// 0x0053D80C slot 0x00
// 0x0053D808 (deleting dtor)
pead::HeapMgr::~HeapMgr()
{
    // empty (in the original too)
}

// 0x00749CC8 | nintendogs:bytes [tier A]
pead::Heap* pead::HeapMgr::getCurrentHeap() const
{
    return ThreadMgr::s_pInstance->getCurrentThread()->mCurrentHeap;
}

// 0x00749CE8 (name after sead)
pead::Heap* pead::HeapMgr::findContainHeap(const void* ptr) const
{
    ScopedLock lock(getHeapTreeLockCS_());
    for (Heap** it = s_RootHeaps.begin(); it != s_RootHeaps.end(); it++) {
        Heap* found = (*it)->findContainHeap_(ptr);
        if (found != nullptr) {
            return found;
        }
    }
    for (Heap** it = s_IndependentHeaps.begin(); it != s_IndependentHeaps.end(); it++) {
        Heap* found = (*it)->findContainHeap_(ptr);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}
} // namespace pead
