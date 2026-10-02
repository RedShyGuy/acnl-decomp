#include "nw/ut/ut_HeapBase.h"
#include "nw/ut/ut_FrameHeap.h"

namespace nw {
namespace ut {
// ctor candidate(s) 0x0013B7A0 (unverified)
nw::ut::FrameHeap::FrameHeap()
{
}

// 0x0048BB54 slot 0x00 | virtual slot, introduced by nw::ut::HeapBase
void nw::ut::FrameHeap::vf_0x00()
{
}

// 0x0048BB44 slot 0x04 | virtual slot, introduced by nw::ut::HeapBase
void nw::ut::FrameHeap::vf_0x04()
{
}

// 0x0013B76C | nintendogs:bytes [tier A]
void nw::ut::FrameHeap::Create(void*, unsigned, unsigned short)
{
}

// 0x0013EF20 | nintendogs:bytes [tier A]
void nw::ut::FrameHeap::FreeByState(unsigned)
{
}

// 0x0013EFC4 | nintendogs:bytes [tier A]
void nw::ut::FrameHeap::RecordState(unsigned)
{
}

// 0x0013F050 | nintendogs:bytes [tier A]
void nw::ut::FrameHeap::Alloc(unsigned, int)
{
}

// 0x00140DE8 | nintendogs:bytes [tier A]
void nw::ut::FrameHeap::Free(int)
{
}

// 0x00140F6C | nintendogs:callgraph [tier A]
void nw::ut::FrameHeap::Destroy()
{
}

} // namespace ut
} // namespace nw
