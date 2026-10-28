#include "sead/seadIDisposer.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"

namespace sead {
// 0x0013378C (name is ours)
sead::IDisposer::IDisposer(Heap* disposerHeap, HeapNullOption option) : mDisposerHeap(disposerHeap)
{
    if (mDisposerHeap == nullptr) {
        switch (option) {
        case HEAP_NULL_OPTION_DEFAULT:
        case HEAP_NULL_OPTION_FIND_CONTAIN_HEAP:
            if (HeapMgr::s_pInstance == nullptr) {
                return;
            }
            mDisposerHeap = HeapMgr::s_pInstance->findContainHeap(this);
            break;
        case HEAP_NULL_OPTION_CURRENT_HEAP:
            if (HeapMgr::s_pInstance == nullptr) {
                return;
            }
            mDisposerHeap = HeapMgr::s_pInstance->getCurrentHeap();
            break;
        case HEAP_NULL_OPTION_NONE:
        default:
            return;
        }
        if (mDisposerHeap == nullptr) {
            return;
        }
    }
    mDisposerHeap->appendDisposer_(this);
}

// 0x00133834 (name is ours)
sead::IDisposer::IDisposer() : mDisposerHeap(nullptr)
{
    if (HeapMgr::s_pInstance == nullptr) {
        return;
    }
    mDisposerHeap = HeapMgr::s_pInstance->findContainHeap(this);
    if (mDisposerHeap != nullptr) {
        mDisposerHeap->appendDisposer_(this);
    }
}

// 0x0013F6E0 slot 0x00 | nintendogs:callseq-callee
// 0x005629A8 (deleting dtor)
sead::IDisposer::~IDisposer()
{
    if (mDisposerHeap == s_pDestructedHeap) {
        return;
    }
    if (mDisposerHeap != nullptr) {
        mDisposerHeap->removeDisposer_(this);
    }
    mDisposerHeap = s_pDestructedHeap;
}

} // namespace sead
