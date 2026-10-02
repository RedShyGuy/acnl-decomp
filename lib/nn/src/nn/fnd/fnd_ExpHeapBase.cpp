#include "nn/fnd/fnd_HeapBase.h"
#include "nn/fnd/fnd_ExpHeapBase.h"

namespace nn {
namespace fnd {
// ctor address unknown
nn::fnd::ExpHeapBase::ExpHeapBase()
{
}

// 0x003523A4 slot 0x00 | virtual slot, introduced by nn::fnd::HeapBase
void nn::fnd::ExpHeapBase::vf_0x00()
{
}

// 0x00352360 slot 0x04 | virtual slot, introduced by nn::fnd::HeapBase
void nn::fnd::ExpHeapBase::vf_0x04()
{
}

// 0x00352340 slot 0x08 | slot vf_0x08 of nn::fnd::HeapBase
void nn::fnd::ExpHeapBase::FreeV(void*)
{
}

// 0x00729710 slot 0x0C | virtual slot, introduced by nn::fnd::HeapBase
void nn::fnd::ExpHeapBase::vf_0x0C()
{
}

// 0x00729700 slot 0x10 | virtual slot, introduced by nn::fnd::HeapBase
void nn::fnd::ExpHeapBase::vf_0x10()
{
}

// 0x0072971C slot 0x14 | virtual slot, introduced by nn::fnd::HeapBase
void nn::fnd::ExpHeapBase::vf_0x14()
{
}

// 0x007296E0 slot 0x18 | fefates:bytes
void nn::fnd::ExpHeapBase::HasAddress(const void*) const
{
}

// 0x0011FF0C | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Initialize(unsigned, unsigned, unsigned)
{
}

// 0x001368EC | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Invalidate()
{
}

// 0x00136918 | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Allocate(unsigned, int, unsigned char, nn::fnd::ExpHeapBase::AllocationMode, bool)
{
}

// 0x0013697C | nintendogs:bytes [tier A]
void nn::fnd::ExpHeapBase::Finalize()
{
}

// 0x0013EC84 | nintendogs:callgraph [tier A]
void nn::fnd::ExpHeapBase::Free(void*)
{
}

} // namespace fnd
} // namespace nn
