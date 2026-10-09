#include "nn/fnd/fnd_ExpHeapBase.h"
#include "nn/dbg/dbg_Api.h"

namespace nn {
namespace fnd {
// 0x003523A4 slot 0x00
// 0x00352360 slot 0x04 (deleting dtor)
nn::fnd::ExpHeapBase::~ExpHeapBase()
{
    Finalize();
}

// 0x00352340 slot 0x08
void nn::fnd::ExpHeapBase::FreeV(void* p)
{
    detail::FreeToHeap(&mImpl, p);
    mAllocationCount--;
}

// 0x0011FF0C | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Initialize(uptr address, size_t size, bit32 option)
{
    if (detail::CreateHeap(&mImpl, reinterpret_cast<void*>(address), size, option) == 0) {
        nndbgPanic();
    }
    mAllocationCount = 0;
}

// 0x001368EC | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Invalidate()
{
    mAllocationCount = 0;
    if (mImpl.signature != 0) {
        detail::DestroyHeap(&mImpl);
        mImpl.signature = 0;
    }
}

// 0x00136918 | nintendogs:bytes [tier A]
void* nn::fnd::ExpHeapBase::Allocate(size_t size, s32 alignment, u8 groupId, AllocationMode mode, bool useMargin)
{
    detail::SetGroupIDForHeap(&mImpl, groupId);
    detail::SetAllocModeForHeap(&mImpl, mode);
    detail::UseMarginOfAlignmentForHeap(&mImpl, useMargin);
    void* p = detail::AllocFromHeap(&mImpl, size, alignment);
    if (p != 0) {
        mAllocationCount++;
    }
    return p;
}

// 0x0013697C | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Finalize()
{
    if (mImpl.signature != 0) {
        detail::DestroyHeap(&mImpl);
        mImpl.signature = 0;
    }
}

// 0x0013EC84 | nintendogs:callgraph [tier A]
void nn::fnd::ExpHeapBase::Free(void* p)
{
    detail::FreeToHeap(&mImpl, p);
    mAllocationCount--;
}

} // namespace fnd
} // namespace nn
