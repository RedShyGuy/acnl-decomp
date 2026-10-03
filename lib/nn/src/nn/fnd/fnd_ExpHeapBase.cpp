#include "nn/fnd/fnd_HeapBase.h"
#include "nn/fnd/fnd_ExpHeapBase.h"

namespace nn {
namespace fnd {
// ctor address unknown
nn::fnd::ExpHeapBase::ExpHeapBase()
{
}

// 0x003523A4 slot 0x00
// 0x00352360 slot 0x04 (deleting dtor)
nn::fnd::ExpHeapBase::~ExpHeapBase()
{
}

// 0x00352340 slot 0x08
void nn::fnd::ExpHeapBase::FreeV(void*)
{
}

// 0x00729710 slot 0x0C (name is ours)
void* nn::fnd::ExpHeapBase::GetHeapStart() const
{
}

// 0x00729700 slot 0x10 (name is ours)
size_t nn::fnd::ExpHeapBase::GetHeapSize() const
{
}

// 0x0072971C slot 0x14 (name is ours)
void nn::fnd::ExpHeapBase::PrintState()
{
}

// 0x007296E0 slot 0x18 | fefates:bytes
bool nn::fnd::ExpHeapBase::HasAddress(const void*) const
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
