#include "nw/ut/ut_HeapNode.h"
#include "nw/ut/ut_HeapBase.h"

namespace nw {
namespace ut {
// ctor address unknown
nw::ut::HeapBase::HeapBase()
{
}

// 0x0048BA90 slot 0x00 | virtual slot, introduced by nw::ut::HeapBase
void nw::ut::HeapBase::vf_0x00()
{
}

// 0x0048BA80 slot 0x04 | virtual slot, introduced by nw::ut::HeapBase
void nw::ut::HeapBase::vf_0x04()
{
}

// 0x0013EE80 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::Initialize(unsigned, void*, void*, unsigned short)
{
}

// 0x00140E78 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::FillFreeMemory(void*, unsigned)
{
}

// 0x00140EA4 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::FillAllocMemory(void*, unsigned)
{
}

// 0x0014387C | nintendogs:bytes [tier A]
void nw::ut::HeapBase::FindContainHeap(nw::ut::LinkList<nw::ut::HeapBase, (long)4>*, const void*)
{
}

// 0x001438D4 | nintendogs:bytes [tier A]
void nw::ut::HeapBase::Finalize()
{
}

} // namespace ut
} // namespace nw
