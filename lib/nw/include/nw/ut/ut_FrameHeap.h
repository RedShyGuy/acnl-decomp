#pragma once

#include "decomp.h"
#include "nw/ut/ut_HeapBase.h"

namespace nw {
namespace ut {
// RTTI N2nw2ut9FrameHeapE @ 0x008D0544
// vtable 0x00902354 (vptr 0x0090235C), offset_to_top 0, 2 entries
class FrameHeap : public ::nw::ut::HeapBase
{
public:
    FrameHeap(); // ctor candidate(s) 0x0013B7A0 (unverified)
    virtual void vf_0x00(); // 0x0048BB54 slot 0x00 | virtual slot, introduced by nw::ut::HeapBase
    virtual void vf_0x04(); // 0x0048BB44 slot 0x04 | virtual slot, introduced by nw::ut::HeapBase
    void Create(void*, unsigned, unsigned short); // 0x0013B76C | nintendogs:bytes [tier A]
    void FreeByState(unsigned); // 0x0013EF20 | nintendogs:bytes [tier A]
    void RecordState(unsigned); // 0x0013EFC4 | nintendogs:bytes [tier A]
    void Alloc(unsigned, int); // 0x0013F050 | nintendogs:bytes [tier A]
    void Free(int); // 0x00140DE8 | nintendogs:bytes [tier A]
    void Destroy(); // 0x00140F6C | nintendogs:callgraph [tier A]
};
} // namespace ut
} // namespace nw
