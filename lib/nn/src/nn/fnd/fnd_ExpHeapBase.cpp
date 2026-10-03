#include "nn/fnd/fnd_HeapBase.h"
#include "nn/fnd/fnd_ExpHeapBase.h"
#include "nn/fnd/detail/detail_Api.h"

namespace nn {
namespace fnd {
// 0x003523A4 slot 0x00
// 0x00352360 slot 0x04 (deleting dtor)
nn::fnd::ExpHeapBase::~ExpHeapBase()
{
}

// 0x00352340 slot 0x08
void nn::fnd::ExpHeapBase::FreeV(void* p)
{
    detail::FreeToHeap(reinterpret_cast<detail::ExpHeapImpl*>(&mImplHead), p);
    mAllocationCount--;
}

// 0x0011FF0C | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Initialize(uptr address, size_t size, bit32 option)
{
}

// 0x001368EC | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Invalidate()
{
}

// 0x00136918 | nintendogs:bytes [tier A]
void* nn::fnd::ExpHeapBase::Allocate(size_t size, s32 alignment, u8 groupId, AllocationMode mode, bool useMargin)
{
}

// 0x0013697C | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Finalize()
{
}

// 0x0013EC84 | nintendogs:callgraph [tier A]
void nn::fnd::ExpHeapBase::Free(void* p)
{
}

} // namespace fnd
} // namespace nn
