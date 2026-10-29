#include "pead/peadHeap.h"
#include "pead/peadHeapMgr.h"
#include "pead/peadIDisposer.h"

namespace pead {
// 0x0097FA20
pead::Heap* pead::IDisposer::s_pDestructedHeap = reinterpret_cast<Heap*>(1);

// 0x0053DB3C (name after sead)
pead::IDisposer::IDisposer() : mDisposerHeap(nullptr)
{
    if (HeapMgr::s_pInstance == nullptr) {
        return;
    }
    mDisposerHeap = HeapMgr::s_pInstance->findContainHeap(this);
    if (mDisposerHeap != nullptr) {
        mDisposerHeap->appendDisposer_(this);
    }
}

// 0x0053DBDC slot 0x00 | slot vf_0x00 of pead::IDisposer
// 0x0053DB90 (deleting dtor)
pead::IDisposer::~IDisposer()
{
    if (mDisposerHeap == s_pDestructedHeap) {
        return;
    }
    if (mDisposerHeap != nullptr) {
        mDisposerHeap->removeDisposer_(this);
    }
    mDisposerHeap = s_pDestructedHeap;
}
} // namespace pead
